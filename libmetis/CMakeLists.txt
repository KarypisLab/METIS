# Add this directory for internal users.
set(METIS_NAME "metis" CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Find sources.
file(GLOB metis_sources *.h *.c)

# Build libmetis.
add_library(${METIS_NAME} ${METIS_LIBRARY_TYPE} ${metis_sources})

if(GKLIB_NAME)
    target_link_libraries(${METIS_NAME} ${GKLIB_NAME})
    add_dependencies(${METIS_NAME} ${GKLIB_NAME})
endif()

if(METIS_INSTALL)
  install(TARGETS ${METIS_NAME}
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib
    ARCHIVE DESTINATION lib)
endif()
