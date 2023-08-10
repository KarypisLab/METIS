cmake_minimum_required(VERSION 2.8)
project(METIS C)

set(METIS_SHARED FALSE CACHE BOOL "build a shared library")
set(METIS_BUILD_PROGRAMS TRUE CACHE BOOL "build metis programs")

if(MSVC)
  set(METIS_INSTALL FALSE)
else()
  set(METIS_INSTALL TRUE)
endif()

# Configure libmetis library.
if(METIS_SHARED)
  set(METIS_LIBRARY_TYPE SHARED)
else()
  set(METIS_LIBRARY_TYPE STATIC)
endif(METIS_SHARED)

include(conf/gkbuild.cmake)

# METIS' custom options
option(METIS_IDX64 "enable 64 bit ints" OFF)
option(METIS_REAL64 "enable 64 bit floats (i.e., double)" OFF)
if(METIS_IDX64)
    set(IDXTYPEWIDTH 64)
else()
    set(IDXTYPEWIDTH 32)
endif(METIS_IDX64)
if(METIS_REAL64)
    set(REALTYPEWIDTH 64)
else()
    set(REALTYPEWIDTH 32)
endif(METIS_REAL64)
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/include/metis.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/metis.h"
    @ONLY
)

# Add include directories.
# i.e., the -I equivalent
#include_directories(build/xinclude)
include_directories(${GKLIB_PATH}/include)
include_directories(${CMAKE_INSTALL_PREFIX}/include)

# List of paths that the compiler will search for library files.
# i.e., the -L equivalent
link_directories(${GKLIB_PATH}/lib)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

# Recursively look for CMakeLists.txt in subdirs.
#add_subdirectory("build/xinclude")
add_subdirectory("libmetis")
if(METIS_BUILD_PROGRAMS)
	add_subdirectory("programs")
endif(METIS_BUILD_PROGRAMS)
