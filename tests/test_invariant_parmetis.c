#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
 * Security property: When processing graph adjacency structures,
 * all vertex index accesses must remain within declared graph bounds.
 * No edge reference should allow access to vwgt[i] or rinfo[i].edegrees
 * at indices beyond the declared number of vertices.
 */

/* Minimal simulation of the graph structures used in parmetis.c */
typedef struct {
    int edegrees[2]; /* from=0, to=1 */
    int id;
    int ed;
} ckrinfo_t;

typedef struct {
    int nvtxs;       /* declared number of vertices */
    int ncon;        /* number of constraints */
    int *xadj;       /* adjacency index array, size nvtxs+1 */
    int *adjncy;     /* adjacency list */
    int *vwgt;       /* vertex weights */
    ckrinfo_t *rinfo;/* refinement info */
    int *where;      /* partition assignment */
} graph_t;

/* Simulate the bounds-checking logic that MUST exist */
static int validate_graph_bounds(graph_t *graph) {
    if (!graph) return 0;
    if (graph->nvtxs <= 0) return 0;
    if (!graph->xadj || !graph->adjncy || !graph->vwgt || !graph->rinfo || !graph->where)
        return 0;

    /* Check that all adjacency references are within bounds */
    for (int i = 0; i < graph->nvtxs; i++) {
        int start = graph->xadj[i];
        int end   = graph->xadj[i + 1];

        /* xadj entries must be non-negative and ordered */
        if (start < 0 || end < start) return 0;

        for (int j = start; j < end; j++) {
            int neighbor = graph->adjncy[j];
            /* INVARIANT: every neighbor index must be within [0, nvtxs) */
            if (neighbor < 0 || neighbor >= graph->nvtxs) return 0;
        }

        /* where[] must be a valid partition id (0 or 1 for 2-way) */
        if (graph->where[i] < 0 || graph->where[i] > 1) return 0;
    }
    return 1;
}

/* Simulate the rpqInsert computation with bounds checking */
static int safe_compute_gain(graph_t *graph, int vertex, int from, int *gain_out) {
    if (!graph || !gain_out) return 0;
    /* Bounds check on vertex index */
    if (vertex < 0 || vertex >= graph->nvtxs) return 0;
    /* Bounds check on partition index */
    if (from < 0 || from > 1) return 0;
    /* Safe access analogous to: vwgt[i] - rinfo[i].edegrees[from] */
    *gain_out = graph->vwgt[vertex] - graph->rinfo[vertex].edegrees[from];
    return 1;
}

/* Helper: allocate a graph with given parameters */
static graph_t *alloc_graph(int nvtxs, int nedges) {
    graph_t *g = (graph_t *)calloc(1, sizeof(graph_t));
    if (!g) return NULL;
    g->nvtxs  = nvtxs;
    g->xadj   = (int *)calloc(nvtxs + 1, sizeof(int));
    g->adjncy = (int *)calloc(nedges > 0 ? nedges : 1, sizeof(int));
    g->vwgt   = (int *)calloc(nvtxs, sizeof(int));
    g->rinfo  = (ckrinfo_t *)calloc(nvtxs, sizeof(ckrinfo_t));
    g->where  = (int *)calloc(nvtxs, sizeof(int));
    if (!g->xadj || !g->adjncy || !g->vwgt || !g->rinfo || !g->where) {
        free(g->xadj); free(g->adjncy); free(g->vwgt);
        free(g->rinfo); free(g->where); free(g);
        return NULL;
    }
    return g;
}

static void free_graph(graph_t *g) {
    if (!g) return;
    free(g->xadj);
    free(g->adjncy);
    free(g->vwgt);
    free(g->rinfo);
    free(g->where);
    free(g);
}

/* ------------------------------------------------------------------ */
/* Test 1: Out-of-bounds vertex indices in adjacency list are rejected */
/* ------------------------------------------------------------------ */
START_TEST(test_oob_vertex_indices_rejected)
{
    /* Invariant: adjacency entries referencing indices >= nvtxs must be
     * detected as invalid before any array access occurs. */

    /* Each payload encodes (nvtxs, bad_neighbor_index) */
    struct { int nvtxs; int bad_neighbor; } payloads[] = {
        { 4,  4   },   /* exactly nvtxs  */
        { 4,  5   },   /* nvtxs + 1      */
        { 4,  100 },   /* large overshoot */
        { 4, -1   },   /* negative index  */
        { 4, INT_MAX }, /* max int        */
        { 4, INT_MIN }, /* min int        */
        { 1,  1   },   /* single vertex, self-ref out of bounds */
        { 10, 10  },   /* exactly nvtxs  */
        { 10, 999 },   /* far out of bounds */
        { 2, -2   },   /* negative        */
    };
    int num_payloads = (int)(sizeof(payloads) / sizeof(payloads[0]));

    for (int p = 0; p < num_payloads; p++) {
        int nvtxs = payloads[p].nvtxs;
        int bad   = payloads[p].bad_neighbor;

        graph_t *g = alloc_graph(nvtxs, 1);
        ck_assert_ptr_nonnull(g);

        /* Build a graph where vertex 0 has one edge to bad_neighbor */
        g->xadj[0] = 0;
        g->xadj[1] = 1;
        for (int i = 1; i <= nvtxs; i++) g->xadj[i] = (i == 1) ? 1 : 1;
        g->adjncy[0] = bad;

        for (int i = 0; i < nvtxs; i++) {
            g->vwgt[i]  = 1;
            g->where[i] = 0;
            g->rinfo[i].edegrees[0] = 0;
            g->rinfo[i].edegrees[1] = 0;
        }

        /* INVARIANT: validation must reject this graph */
        int valid = validate_graph_bounds(g);
        ck_assert_int_eq(valid, 0);

        free_graph(g);
    }
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 2: Valid graphs pass validation and gain computation succeeds  */
/* ------------------------------------------------------------------ */
START_TEST(test_valid_graph_accepted)
{
    /* Invariant: a well-formed graph must pass validation and allow
     * safe gain computation for every vertex. */

    int nvtxs = 6;
    /* Simple path: 0-1-2-3-4-5 */
    int edges[][2] = {{0,1},{1,0},{1,2},{2,1},{2,3},{3,2},{3,4},{4,3},{4,5},{5,4}};
    int nedges = 10;

    graph_t *g = alloc_graph(nvtxs, nedges);
    ck_assert_ptr_nonnull(g);

    /* Count degrees */
    int deg[6] = {1,2,2,2,2,1};
    int pos = 0;
    g->xadj[0] = 0;
    for (int i = 0; i < nvtxs; i++) {
        g->xadj[i+1] = g->xadj[i] + deg[i];
    }

    /* Fill adjacency */
    int adj_fill[6] = {0};
    for (int e = 0; e < nedges; e++) {
        int u = edges[e][0];
        int v = edges[e][1];
        g->adjncy[g->xadj[u] + adj_fill[u]] = v;
        adj_fill[u]++;
    }

    for (int i = 0; i < nvtxs; i++) {
        g->vwgt[i]  = i + 1;
        g->where[i] = i % 2;
        g->rinfo[i].edegrees[0] = 1;
        g->rinfo[i].edegrees[1] = 1;
    }

    /* INVARIANT: valid graph must pass */
    ck_assert_int_eq(validate_graph_bounds(g), 1);

    /* INVARIANT: gain computation must succeed for all vertices */
    for (int i = 0; i < nvtxs; i++) {
        int gain = 0;
        int from = g->where[i];
        int ok = safe_compute_gain(g, i, from, &gain);
        ck_assert_int_eq(ok, 1);
        /* gain = vwgt[i] - rinfo[i].edegrees[from] */
        ck_assert_int_eq(gain, g->vwgt[i] - g->rinfo[i].edegrees[from]);
    }

    free_graph(g);
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 3: Out-of-bounds vertex index in gain computation is rejected  */
/* ------------------------------------------------------------------ */
START_TEST(test_oob_gain_computation_rejected)
{
    /* Invariant: safe_compute_gain must never access memory beyond
     * the declared graph size, regardless of the vertex index supplied. */

    int adversarial_vertices[] = {
        -1, -100, INT_MIN,
        5,   /* exactly nvtxs for a 5-vertex graph */
        6, 100, INT_MAX
    };
    int num_adv = (int)(sizeof(adversarial_vertices) / sizeof(adversarial_vertices[0]));

    int nvtxs = 5;
    graph_t *g = alloc_graph(nvtxs, 0);
    ck_assert_ptr_nonnull(g);

    for (int i = 0; i < nvtxs; i++) {
        g->xadj[i]   = 0;
        g->xadj[i+1] = 0;
        g->vwgt[i]   = 10;
        g->where[i]  = 0;
        g->rinfo[i].edegrees[0] = 2;
        g->rinfo[i].edegrees[1] = 3;
    }

    for (int p = 0; p < num_adv; p++) {
        int bad_vtx = adversarial_vertices[p];
        int gain = 0;
        /* INVARIANT: must return failure, not perform OOB access */
        int ok = safe_compute_gain(g, bad_vtx, 0, &gain);
        ck_assert_int_eq(ok, 0);
    }

    /* Also test bad partition index */
    int bad_parts[] = { -1, 2, 3, INT_MAX, INT_MIN };
    int num_bp = (int)(sizeof(bad_parts) / sizeof(bad_parts[0]));
    for (int p = 0; p < num_bp; p++) {
        int gain = 0;
        int ok = safe_compute_gain(g, 0, bad_parts[p], &gain);
        ck_assert_int_eq(ok, 0);
    }

    free_graph(g);
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 4: Malformed xadj (negative or decreasing) is rejected        */
/* ------------------------------------------------------------------ */
START_TEST(test_malformed_xadj_rejected)
{
    /* Invariant: xadj arrays with invalid (negative or non-monotone)
     * entries must be detected before any adjacency traversal. */

    int nvtxs = 4;
    graph_t *g = alloc_graph(nvtxs, 4);
    ck_assert_ptr_nonnull(g);

    for (int i = 0; i < nvtxs; i++) {
        g->vwgt[i]  = 1;
        g->where[i] = 0;
        g->rinfo[i].edegrees[0] = 0;
        g->rinfo[i].edegrees[1] = 0;
    }
    /* Valid neighbors */
    g->adjncy[0] = 1; g->adjncy[1] = 2;
    g->adjncy[2] = 0; g->adjncy[3] = 3;

    /* Case 1: negative xadj entry */
    g->xadj[0] = -1; g->xadj[1] = 1; g->xadj[2] = 2;
    g->xadj[3] = 3;  g->xadj[4] = 4;
    ck_assert_int_eq(validate_graph_bounds(g), 0);

    /* Case 2: decreasing xadj (end < start) */
    g->xadj[0] = 2; g->xadj[1] = 1; g->xadj[2] = 2;
    g->xadj[3] = 3; g->xadj[4] = 4;
    ck_assert_int_eq(validate_graph_bounds(g), 0);

    /* Case 3: xadj[0]=0 but xadj[1] wraps (INT_MAX) */
    g->xadj[0] = 0; g->xadj[1] = INT_MAX; g->xadj[2] = 2;
    g->xadj[3] = 3; g->xadj[4] = 4;
    ck_assert_int_eq(validate_graph_bounds(g), 0);

    free_graph(g);
}
END_TEST

/* ------------------------------------------------------------------ */
/* Test 5: where[] out-of-range partition values are rejected          */
/* ------------------------------------------------------------------ */
START_TEST(test_invalid_partition_assignment_rejected)
{
    /* Invariant: partition assignments outside [0,1] for 2-way
     * partitioning must be detected to prevent edegrees[from] OOB. */

    int bad_where[] = { -1, 2, 3, 100, INT_MAX, INT_MIN, -100 };
    int num_bw = (int)(sizeof(bad_where) / sizeof(bad_where[0]));

    int nvtxs = 3;
    graph_t *g = alloc_graph(nvtxs, 2);
    ck_assert_ptr_nonnull(g);

    /* Simple graph: 0-1-2 */
    g->xadj[0] = 0; g->xadj[1] =