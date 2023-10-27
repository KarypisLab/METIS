add_executable(METIS_mtest mtest.c)
set_target_properties(METIS_mtest PROPERTIES
		OUTPUT_NAME mtest
)
target_link_libraries(METIS_mtest PRIVATE METIS_METIS GKlib::GKlib)
# TODO: Decouple from the Programs targets
# TODO: cli interface is not actually implemented
target_link_libraries(METIS_mtest PRIVATE METIS_cli)

target_include_directories(METIS_mtest PRIVATE
		# TODO: Should not strongly couple the tests to the internal headers
		${PROJECT_SOURCE_DIR}/src/libmetis
)

# TODO: Link mtest to actual add_test
