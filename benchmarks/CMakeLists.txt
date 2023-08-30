# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)
project(cmdout_benchmarks CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

find_package(Threads REQUIRED)
find_package(benchmark CONFIG)
if(NOT benchmark_FOUND)
    message(STATUS "Using CMake Version ${CMAKE_VERSION}")
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.11.0")
        # User can fetch googlebenchmark
        message(STATUS "Downloading GoogleBenchmark")
        include(FetchContent)

        # disable tests
        set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
        # Do not build and run googlebenchmark tests
        #FetchContent_Declare(googlebenchmark GIT_REPOSITORY https://github.com/google/benchmark.git GIT_TAG v1.6.0)
        FetchContent_Declare(googlebenchmark URL https://github.com/google/benchmark/archive/refs/tags/v1.6.0.tar.gz)
        FetchContent_MakeAvailable(googlebenchmark)
    else()
        message(FATAL_ERROR "GoogleBenchmark is missing. Use CMake >= 3.11 or download it")
    endif()
endif()

###########################
# target benchmarks
###########################
add_executable(benchmarks "benchmarks.cpp")
cmdout_enable_warnings(benchmarks)
target_link_libraries(benchmarks PRIVATE myvas::cmdout)