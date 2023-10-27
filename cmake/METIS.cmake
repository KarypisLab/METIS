include_guard()

function(METIS_FetchContent_MakeAvailable)
	#[===[.md
	# METIS_FetchContent_MakeAvailable

	Calls FetchContent<PackageName>_Before.cmake and FetchContent<PackageName>_After.cmake for each project

	]===]

	list(APPEND CMAKE_MESSAGE_CONTEXT "METIS_FetchContent_MakeAvailable")
	set(ARGS_Options "")
	set(ARGS_OneValue "MODULES_PATH;PREFIX;BEFORE_SUFFIX;AFTER_SUFFIX")
	set(ARGS_MultiValue "")
	cmake_parse_arguments(ARGS "${ARGS_Options}" "${ARGS_OneValue}" "${ARGS_MultiValue}" ${ARGN})

	if(NOT DEFINED ARGS_MODULES_PATH)
		set(ARGS_MODULES_PATH ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/compat)
	endif ()
	if(NOT DEFINED ARGS_PREFIX)
		set(ARGS_PREFIX FetchContent)
	endif ()
	if(NOT DEFINED ARGS_BEFORE_SUFFIX)
		set(ARGS_BEFORE_SUFFIX _Before)
	endif ()
	if(NOT DEFINED ARGS_AFTER_SUFFIX)
		set(ARGS_AFTER_SUFFIX _After)
	endif ()

	foreach (pkg IN LISTS ARGS_UNPARSED_ARGUMENTS)
		include(${ARGS_MODULES_PATH}/${ARGS_PREFIX}${pkg}${ARGS_BEFORE_SUFFIX}.cmake OPTIONAL)
		FetchContent_MakeAvailable(${pkg})
		include(${ARGS_MODULES_PATH}/${ARGS_PREFIX}${pkg}${ARGS_AFTER_SUFFIX}.cmake OPTIONAL)
	endforeach ()
endfunction()