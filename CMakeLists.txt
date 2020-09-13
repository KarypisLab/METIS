cmake_minimum_required(VERSION 2.8)
project(METIS C)

set(GKLIB_PATH "${CMAKE_SOURCE_DIR}/GKlib" CACHE PATH "path to GKlib")
set(SHARED FALSE CACHE BOOL "build a shared library")

if(MSVC)
  set(METIS_INSTALL FALSE)
else()
  set(METIS_INSTALL TRUE)
endif()

# Configure libmetis library.
if(SHARED)
  set(METIS_LIBRARY_TYPE SHARED)
else()
  set(METIS_LIBRARY_TYPE STATIC)
endif(SHARED)

include(${GKLIB_PATH}/GKlibSystem.cmake)

# METIS' custom options
option(METIS_IDX64 "enable 64 bit ints" OFF)
option(METIS_REAL64 "enable 64 bit floats (i.e., double)" OFF)
if (METIS_IDX64)
  set(METIS_IDXTYPEWIDTH 64)
else ()
  set(METIS_IDXTYPEWIDTH 32)
endif ()
if (METIS_REAL64)
  set(METIS_REALTYPEWIDTH 64)
else ()
  set(METIS_REALTYPEWIDTH 32)
endif ()

# Add include directories.
include_directories(${GKLIB_PATH})
configure_file(${PROJECT_SOURCE_DIR}/include/metis.h.in
               ${PROJECT_BINARY_DIR}/include/metis.h)
include_directories(${PROJECT_BINARY_DIR}/include)
if (METIS_INSTALL)
  install(FILES ${PROJECT_BINARY_DIR}/include/metis.h DESTINATION include)
endif ()

# Recursively look for CMakeLists.txt in subdirs.
add_subdirectory("libmetis")
add_subdirectory("programs")
