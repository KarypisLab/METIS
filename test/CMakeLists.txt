function(METIS_add_test test)
	#[===[.md
	# METIS_add_test

	Internal helper for adding GKlib tests

	## Synopsis
	```cmake
	METIS_add_test(<name>
			[TEST_NAME <test_name>]
			[TARGET <target>]
			[LABELS <label1> <label2>])
	```

	## Options

	`<name>`
	  Path to the CMake project to be executed relative to `${CMAKE_CURRENT_SOURCE_DIR}`

	`TEST_NAME` [Default: `<name>`]
	  Name for the test to be used as the ctest name

	`LABELS`
	  Additional labels to be added

	]===]

	list(APPEND CMAKE_MESSAGE_CONTEXT METIS_add_test)
	set(ARGS_Options)
	set(ARGS_OneValue
			TEST_NAME
	)
	set(ARGS_MultiValue
			LABELS
	)
	cmake_parse_arguments(PARSE_ARGV 1 ARGS "${ARGS_Options}" "${ARGS_OneValue}" "${ARGS_MultiValue}")
	# Check required/optional arguments
	if (ARGC LESS 1)
		message(FATAL_ERROR "Missing test name")
	endif ()
	if (NOT DEFINED ARGS_TEST_NAME)
		set(ARGS_TEST_NAME ${test})
	endif ()

	add_test(NAME ${ARGS_TEST_NAME}
			COMMAND ${CMAKE_CTEST_COMMAND} --build-and-test ${CMAKE_CURRENT_SOURCE_DIR}/${test}
			${CMAKE_CURRENT_BINARY_DIR}/${test}
			--build-generator "${CMAKE_GENERATOR}"
			--build-options -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
			# Generated Config file point to binary targets until it is installed
			-DFETCHCONTENT_TRY_FIND_PACKAGE_MODE=ALWAYS
			-DMETIS_ROOT=${PROJECT_BINARY_DIR}
			# TODO: Implement recursive ctest and remove --notests flag
			--test-command ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_CURRENT_BINARY_DIR}/${test} --no-tests=ignore
	)
	set_tests_properties(${ARGS_TEST_NAME} PROPERTIES
			LABELS "${ARGS_LABELS}"
	)
endfunction()

foreach (test IN ITEMS
)
	# TODO: Implement some regression tests with METIS_add_test
endforeach ()
# TODO: Decouple mtest to be independent from other dependencies
if (METIS_PROGRAMS)
	# TODO: Currently reusing METIS_cli target
#	add_subdirectory(mtest)
endif ()
