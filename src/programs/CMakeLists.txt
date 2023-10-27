# Define a simple cli helper target
add_library(METIS_cli OBJECT)

# Define program targets
add_executable(METIS_gpmetis)
add_executable(METIS::gpmetis ALIAS METIS_gpmetis)
set_target_properties(METIS_gpmetis PROPERTIES
		EXPORT_NAME gpmetis
		OUTPUT_NAME gpmetis
)
add_executable(METIS_ndmetis)
add_executable(METIS::ndmetis ALIAS METIS_ndmetis)
set_target_properties(METIS_ndmetis PROPERTIES
		EXPORT_NAME ndmetis
		OUTPUT_NAME ndmetis
)
add_executable(METIS_mpmetis)
add_executable(METIS::mpmetis ALIAS METIS_mpmetis)
set_target_properties(METIS_mpmetis PROPERTIES
		EXPORT_NAME mpmetis
		OUTPUT_NAME mpmetis
)
add_executable(METIS_m2gmetis)
add_executable(METIS::m2gmetis ALIAS METIS_m2gmetis)
set_target_properties(METIS_m2gmetis PROPERTIES
		EXPORT_NAME m2gmetis
		OUTPUT_NAME m2gmetis
)
add_executable(METIS_graphchk)
add_executable(METIS::graphchk ALIAS METIS_graphchk)
set_target_properties(METIS_graphchk PROPERTIES
		EXPORT_NAME graphchk
		OUTPUT_NAME graphchk
)
add_executable(METIS_cmpfillin)
add_executable(METIS::cmpfillin ALIAS METIS_cmpfillin)
set_target_properties(METIS_cmpfillin PROPERTIES
		EXPORT_NAME cmpfillin
		OUTPUT_NAME cmpfillin
)

# Actual target definition
target_sources(METIS_cli PRIVATE
		io.c
		stat.c
		smbfactor.c
)
target_link_libraries(METIS_cli PUBLIC GKlib::GKlib METIS_METIS)
# Add include_directories for mtest to use
target_include_directories(METIS_cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(METIS_gpmetis PRIVATE
		gpmetis.c
		cmdline_gpmetis.c
)
target_link_libraries(METIS_gpmetis PRIVATE METIS_cli)
target_sources(METIS_ndmetis PRIVATE
		ndmetis.c
		cmdline_ndmetis.c
)
target_link_libraries(METIS_ndmetis PRIVATE METIS_cli)
target_sources(METIS_mpmetis PRIVATE
		mpmetis.c
		cmdline_mpmetis.c
)
target_link_libraries(METIS_mpmetis PRIVATE METIS_cli)
target_sources(METIS_m2gmetis PRIVATE
		m2gmetis.c
		cmdline_m2gmetis.c
)
target_link_libraries(METIS_m2gmetis PRIVATE METIS_cli)
target_sources(METIS_graphchk PRIVATE
		graphchk.c
)
target_link_libraries(METIS_graphchk PRIVATE METIS_cli)
target_sources(METIS_cmpfillin PRIVATE
		cmpfillin.c
)
target_link_libraries(METIS_cmpfillin PRIVATE METIS_cli)

# install
if (METIS_INSTALL)
	get_directory_property(PorgramTargets BUILDSYSTEM_TARGETS)
	list(REMOVE_ITEM PorgramTargets METIS_cli)
	install(TARGETS ${PorgramTargets}
			# TODO: This should be moved to a different optional target
			EXPORT METISTargets
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT METIS_Runtime
	)
endif ()
