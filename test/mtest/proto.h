/*
 * Copyright 1997, Regents of the University of Minnesota
 *
 * proto.h
 *
 * This file contains header files
 *
 * Started 10/19/95
 * George
 *
 * $Id: proto.h 5940 2008-12-26 21:58:40Z karypis $
 *
 */

#ifndef _TEST_PROTO_H_
#define _TEST_PROTO_H_

void ReadGraph(graph_t *, char *, idx_t *);

void Test_PartGraph(idx_t, idx_t *, idx_t *);
int  VerifyPart(idx_t, idx_t *, idx_t *, idx_t *, idx_t *, idx_t, idx_t, idx_t *);
int  VerifyWPart(idx_t, idx_t *, idx_t *, idx_t *, idx_t *, idx_t, real_t *, idx_t, idx_t *);
void Test_PartGraphV(idx_t, idx_t *, idx_t *);
int  VerifyPartV(idx_t, idx_t *, idx_t *, idx_t *, idx_t *, idx_t, idx_t, idx_t *);
int  VerifyWPartV(idx_t, idx_t *, idx_t *, idx_t *, idx_t *, idx_t, real_t *, idx_t, idx_t *);
void Test_PartGraphmC(idx_t, idx_t *, idx_t *);
int  VerifyPartmC(idx_t, idx_t, idx_t *, idx_t *, idx_t *, idx_t *, idx_t, real_t *, idx_t, idx_t *);
void Test_ND(idx_t, idx_t *, idx_t *);
int  VerifyND(idx_t, idx_t *, idx_t *);

#endif
