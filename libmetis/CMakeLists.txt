target_sources(METIS_METIS PRIVATE
		auxapi.c
		balance.c
		bucketsort.c
		checkgraph.c
		coarsen.c
		compress.c
		contig.c
		debug.c
		fm.c
		fortran.c
		frename.c
		gklib.c
		graph.c
		initpart.c
		kmetis.c
		kwayfm.c
		kwayrefine.c
		mcutil.c
		mesh.c
		meshpart.c
		minconn.c
		mincover.c
		mmd.c
		ometis.c
		options.c
		parmetis.c
		pmetis.c
		refine.c
		separator.c
		sfm.c
		srefine.c
		stat.c
		timing.c
		util.c
		wspace.c
)
target_link_libraries(METIS_METIS PRIVATE GKlib::GKlib)

# Install
if (METIS_INSTALL)
	install(TARGETS METIS_METIS
			EXPORT METISTargets
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT METIS_Runtime NAMELINK_COMPONENT METIS_Development
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT METIS_Development
			PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT METIS_Development
			RUNTIME DESTINATION ${CMAKE_INSTALL_RUNTIMEDIR} COMPONENT METIS_Runtime
	)
endif ()
