# Add this directory for internal users.
include_directories(.)

# Find sources.
file(GLOB metis_sources *.c)

# Build libmetis.
add_library(metis ${METIS_LIBRARY_TYPE} ${metis_sources})
target_link_libraries(metis GKlib)
if(UNIX)
  target_link_libraries(metis m)
endif()

target_include_directories(metis PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# With GCC LTO, also emit machine code next to the GIMPLE (fat objects) so
# libmetis.a links with non-GCC toolchains and without the LTO plugin (#128).
if(CMAKE_INTERPROCEDURAL_OPTIMIZATION AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(metis PRIVATE -ffat-lto-objects)
endif()

if(METIS_INSTALL)
  install(TARGETS metis
    EXPORT METISTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
