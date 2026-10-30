cmake_minimum_required(VERSION 3.10)
project(METIS C)

include(GNUInstallDirs)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# T3.1: enable link-time / interprocedural optimization when the toolchain
# supports it. Lets the hot k-way/FM refinement loops inline the rpq* priority
# queue ops (instantiated in libmetis/gklib.c) across translation units.
include(CheckIPOSupported)
check_ipo_supported(RESULT METIS_IPO_SUPPORTED OUTPUT METIS_IPO_MSG)
if(METIS_IPO_SUPPORTED)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  message(STATUS "METIS: interprocedural optimization (LTO) enabled")
else()
  message(STATUS "METIS: IPO/LTO not supported: ${METIS_IPO_MSG}")
endif()
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

include(./conf/gkbuild.cmake)

# METIS' custom options
#option(IDX64 "enable 64 bit ints" OFF)
#option(REAL64 "enable 64 bit floats (i.e., double)" OFF)
#if(IDX64)
#  set(METIS_COPTIONS "${METIS_COPTIONS} -DIDXTYPEWIDTH=64")
#else()
#  set(METIS_COPTIONS "${METIS_COPTIONS} -DIDXTYPEWIDTH=32")
#endif(IDX64)
#if(REAL64)
#  set(METIS_COPTIONS "${METIS_COPTIONS} -DREALTYPEWIDTH=64")
#else()
#  set(METIS_COPTIONS "${METIS_COPTIONS} -DREALTYPEWIDTH=32")
#endif(REAL64)
#
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${METIS_COPTIONS}")


# Add include directories.
# i.e., the -I equivalent
include_directories(build/xinclude)
include_directories(${GKLIB_PATH}/include)
include_directories(${CMAKE_INSTALL_PREFIX}/include)

# List of paths that the compiler will search for library files.
# i.e., the -L equivalent
set(METIS_LINK_DIRS)
foreach(prefix ${GKLIB_PATH} ${CMAKE_INSTALL_PREFIX})
  foreach(libdir ${CMAKE_INSTALL_LIBDIR} lib64 lib)
    if(IS_DIRECTORY "${prefix}/${libdir}")
      list(APPEND METIS_LINK_DIRS "${prefix}/${libdir}")
    endif()
  endforeach()
endforeach()
list(REMOVE_DUPLICATES METIS_LINK_DIRS)
link_directories(${METIS_LINK_DIRS})

# Recursively look for CMakeLists.txt in subdirs.
add_subdirectory("build/xinclude")
add_subdirectory("libmetis")
add_subdirectory("programs")

if(METIS_INSTALL)
  file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/include/metis.h METIS_VER_MAJOR_LINE REGEX "#define METIS_VER_MAJOR[ \t]+[0-9]+")
  file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/include/metis.h METIS_VER_MINOR_LINE REGEX "#define METIS_VER_MINOR[ \t]+[0-9]+")
  file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/include/metis.h METIS_VER_SUBMINOR_LINE REGEX "#define METIS_VER_SUBMINOR[ \t]+[0-9]+")
  string(REGEX REPLACE ".*METIS_VER_MAJOR[ \t]+([0-9]+).*" "\\1" METIS_VERSION_MAJOR "${METIS_VER_MAJOR_LINE}")
  string(REGEX REPLACE ".*METIS_VER_MINOR[ \t]+([0-9]+).*" "\\1" METIS_VERSION_MINOR "${METIS_VER_MINOR_LINE}")
  string(REGEX REPLACE ".*METIS_VER_SUBMINOR[ \t]+([0-9]+).*" "\\1" METIS_VERSION_SUBMINOR "${METIS_VER_SUBMINOR_LINE}")

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/METISConfigVersion.cmake
    VERSION "${METIS_VERSION_MAJOR}.${METIS_VERSION_MINOR}.${METIS_VERSION_SUBMINOR}"
    COMPATIBILITY SameMajorVersion
  )
  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/METISConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/METISConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/metis
  )
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/METISConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/METISConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/metis
  )
  install(EXPORT METISTargets
    FILE METISTargets.cmake
    NAMESPACE METIS::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/metis
  )
endif()
