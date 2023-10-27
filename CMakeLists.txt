cmake_minimum_required(VERSION 3.15)
# CMake version compatibility
# TODO: Remove when cmake 3.25 is commonly distributed
if (POLICY CMP0140)
	cmake_policy(SET CMP0140 NEW)
endif ()

#[==============================================================================================[
#                                   Basic project definition                                   #
]==============================================================================================]

list(APPEND CMAKE_MESSAGE_CONTEXT METIS)
project(METIS
		VERSION 5.3.0
		DESCRIPTION "Serial Graph Partitioning and Fill-reducing Matrix Ordering"
		HOMEPAGE_URL https://github.com/KarypisLab/METIS
		LANGUAGES C
)

# Back-porting to PROJECT_IS_TOP_LEVEL to older cmake
# TODO: Remove when requiring cmake >= 3.21
if (NOT DEFINED Spglib_IS_TOP_LEVEL)
	if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		set(PROJECT_IS_TOP_LEVEL ON)
	else ()
		set(PROJECT_IS_TOP_LEVEL OFF)
	endif ()
endif ()

# Specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

#[==============================================================================================[
#                                            Options                                            #
]==============================================================================================]

include(CMakeDependentOption)
include(FeatureSummary)
option(METIS_TESTS "METIS: Build unit tests" ${PROJECT_IS_TOP_LEVEL})
option(METIS_SHARED_LIBS "METIS: Build as a shared library" ${PROJECT_IS_TOP_LEVEL})
option(METIS_INSTALL "METIS: Install project" ${PROJECT_IS_TOP_LEVEL})
option(METIS_PROGRAMS "METIS: Build executable programs" OFF)
add_feature_info(METIS_PROGRAMS METIS_PROGRAMS "Build executable programs")
option(METIS_IDX64 "METIS: Build with 64 bit ints" OFF)
add_feature_info(METIS_IDX64 METIS_IDX64 "Build with 64 bit ints")
option(METIS_REAL64 "METIS: Build with 64 bit floats" OFF)
add_feature_info(METIS_REAL64 METIS_REAL64 "Build with 64 bit floats")

#[==============================================================================================[
#                                     Project configuration                                     #
]==============================================================================================]

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Include basic tools
include(FetchContent)
if (METIS_INSTALL)
	include(CMakePackageConfigHelpers)
	if (UNIX)
		include(GNUInstallDirs)
	endif ()
endif ()

# Define basic parameters
set(BUILD_SHARED_LIBS ${METIS_SHARED_LIBS})


#[==============================================================================================[
#                                       External packages                                       #
]==============================================================================================]

FetchContent_Declare(GKlib
		GIT_REPOSITORY https://github.com/KarypisLab/GKlib
		GIT_TAG master
		FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(GKlib)

include(METIS_PackagesInfo)

feature_summary(
		FILENAME ${CMAKE_CURRENT_BINARY_DIR}/METIS.info
		VAR METIS_Info
		DESCRIPTION "METIS supported libraries"
		FATAL_ON_MISSING_REQUIRED_PACKAGES
		WHAT ALL
)
message(STATUS ${METIS_Info})

#[==============================================================================================[
#                                        Main definition                                        #
]==============================================================================================]

# Main project
add_library(METIS_METIS)
set_target_properties(METIS_METIS PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
		EXPORT_NAME METIS
		OUTPUT_NAME metis
)
add_library(METIS::METIS ALIAS METIS_METIS)

# Main definitions inside libmetis
add_subdirectory(include)
add_subdirectory(libmetis)

if (METIS_PROGRAMS)
	add_subdirectory(programs)
endif ()

# Testing
if (METIS_TESTS)
	enable_testing()
	add_subdirectory(test)
endif ()

#[==============================================================================================[
#                                       Install or Export                                       #
]==============================================================================================]

# Installation
if (METIS_INSTALL)
	# pkg-config files
	configure_file(cmake/metis.pc.in metis.pc @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/metis.pc
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
			COMPONENT METIS_Development
	)

	# cmake export files
	write_basic_package_version_file(
			${CMAKE_CURRENT_BINARY_DIR}/METISConfigVersion.cmake
			VERSION ${PROJECT_VERSION}
			COMPATIBILITY SameMajorVersion
	)
	configure_package_config_file(
			cmake/METISConfig.cmake.in
			${CMAKE_CURRENT_BINARY_DIR}/METISConfig.cmake
			INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/METIS
	)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/METISConfigVersion.cmake
			${CMAKE_CURRENT_BINARY_DIR}/METISConfig.cmake
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/METIS
			COMPONENT METIS_Development
	)
	export(EXPORT METISTargets
			FILE METISTargets.cmake
			NAMESPACE METIS::
	)
	install(EXPORT METISTargets
			FILE METISTargets.cmake
			NAMESPACE METIS::
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/METIS
			COMPONENT METIS_Development
	)
endif ()

# Make project available for FetchContent
if (NOT PROJECT_IS_TOP_LEVEL)
	# Propagate variables
	if (CMAKE_VERSION VERSION_LESS 3.25)
		# TODO: Remove when cmake 3.25 is commonly distributed
		set(METIS_VERSION ${METIS_VERSION} PARENT_SCOPE)
		set(METIS_VERSION_MAJOR ${METIS_VERSION_MAJOR} PARENT_SCOPE)
		set(METIS_VERSION_MINOR ${METIS_VERSION_MINOR} PARENT_SCOPE)
		set(METIS_VERSION_PATCH ${METIS_VERSION_PATCH} PARENT_SCOPE)
		set(METIS_VERSION_TWEAK ${METIS_VERSION_TWEAK} PARENT_SCOPE)
		set(METIS_IDXTYPEWIDTH ${METIS_IDXTYPEWIDTH} PARENT_SCOPE)
		set(METIS_REALTYPEWIDTH ${METIS_REALTYPEWIDTH} PARENT_SCOPE)
	else ()
		return(PROPAGATE
				METIS_VERSION
				METIS_VERSION_MAJOR
				METIS_VERSION_MINOR
				METIS_VERSION_PATCH
				METIS_VERSION_TWEAK
				METIS_IDXTYPEWIDTH
				METIS_REALTYPEWIDTH
		)
	endif ()
endif ()
