# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Myvas Foundation

cmake_minimum_required(VERSION 3.19)
project(cmdout_benchmarks LANGUAGES CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(benchmark)

###########################
# target benchmarks
###########################
add_executable(benchmarks "src/benchmarks.cpp")
target_link_libraries(benchmarks PRIVATE cmdout benchmark::benchmark)
