cmake_minimum_required(VERSION 3.15)

project(mtest
		LANGUAGES C
)

include(FetchContent)
FetchContent_Declare(METIS
		GIT_REPOSITORY https://github.com/KarypisLab/GKlib
		GIT_TAG master
		FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(METIS)

add_executable(mtest mtest.c)
target_link_libraries(mtest PRIVATE METIS::METIS)

# TODO: Add actual add_test using ../../graphs
enable_testing()
