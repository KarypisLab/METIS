# Generate metis.h file with appropriate exported macros
if (METIS_IDX64)
	set(IDXTYPEWIDTH 64)
else ()
	set(IDXTYPEWIDTH 32)
endif ()
if (METIS_REAL64)
	set(REALTYPEWIDTH 64)
else ()
	set(REALTYPEWIDTH 32)
endif ()
configure_file(metis.h.in metis.h @ONLY)

# Configure main target to consume the metis.h
# TODO: Move to FILE_SET for cmake 3.23
set_target_properties(METIS_METIS PROPERTIES
		PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/metis.h
)
target_include_directories(METIS_METIS PUBLIC
		# Add the path to the generated metis.h
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Set namespaced variables to be exported
set(METIS_IDXTYPEWIDTH ${IDXTYPEWIDTH})
set(METIS_REALTYPEWIDTH ${REALTYPEWIDTH})
if (CMAKE_VERSION VERSION_LESS 3.25)
	# TODO: Remove when cmake 3.25 is commonly distributed
	set(METIS_IDXTYPEWIDTH ${METIS_IDXTYPEWIDTH} PARENT_SCOPE)
	set(METIS_REALTYPEWIDTH ${METIS_IDXTYPEWIDTH} PARENT_SCOPE)
else ()
	return(PROPAGATE
			METIS_IDXTYPEWIDTH
			METIS_REALTYPEWIDTH
	)
endif ()
