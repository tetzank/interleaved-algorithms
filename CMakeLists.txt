cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED CMAKE_CXX_FLAGS)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -march=native -std=c++11" CACHE STRING "flags used by the compiler during all build types.")
endif()
# build type defaults to release
if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS) Debug Release RelWithDebInfo MinSizeRel.")
endif()

project(interleavedAlgorithms CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(GBenchmark)


add_executable(gen gen.cpp)

add_executable(binary_search binary_search.cpp)

if(${GBENCHMARK_FOUND})
	add_executable(binary_search_bench binary_search_bench.cpp)
	target_link_libraries(binary_search_bench ${GBENCHMARK_LIBRARIES} pthread)
else()
	message(STATUS "google benchmark not found, skipping benchmark programs.")
endif()
