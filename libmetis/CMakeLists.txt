# Add this directory for internal users.
include_directories(.)
# Find headers.
file(GLOB metis_includes *.h)
# Find sources.
file(GLOB metis_sources *.c)
# Build libmetis.
add_library(metis ${METIS_LIBRARY_TYPE} ${GKlib_sources} ${metis_sources})
if(UNIX)
  target_link_libraries(metis m)
endif()

if(METIS_INSTALL)
  install(FILES ${GKlib_includes}
    DESTINATION include)
  install(FILES ${metis_includes}
    DESTINATION include)
  install(TARGETS metis
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib
    ARCHIVE DESTINATION lib)
endif()
