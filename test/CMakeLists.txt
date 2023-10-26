add_executable(METIS_Test)

set_target_properties(METIS_Test PROPERTIES
		OUTPUT_NAME mtest
)

target_sources(METIS_Test PRIVATE
		mtest.c
)
target_link_libraries(METIS_Test PRIVATE METIS_METIS)

# TODO: Add actual add_test using ../graphs
