# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

###################################
# Find or fetch a CMake package
#
# benchmark >=1.8.3
# benchmark::benchmark
###################################

set(benchmark_VERSION_REQUIRED 1.8.3)

find_package(benchmark ${benchmark_VERSION_REQUIRED} QUIET)
if(benchmark_FOUND)
    message(STATUS "Found benchmark: ${benchmark_VERSION}")
else()
    message(STATUS "CMake package benchmark ${benchmark_VERSION_REQUIRED} or later not found!"
        " Please wait for fetching from https://github.com/google/benchmark"
    )

    include(FetchContent)
    ###############################################
    # Do not build and run googlebenchmark tests!
    ###############################################
    set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
    FetchContent_Declare(benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v${benchmark_VERSION_REQUIRED}.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(benchmark)

    if(benchmark_SOURCE_DIR)
        message(STATUS "Built benchmark: ${benchmark_VERSION_REQUIRED}")
    else()
        if(benchmark_FIND_REQUIRED)
            message(FATAL_ERROR "Failed to build benchmark: ${benchmark_VERSION_REQUIRED}")
        else()
            message(WARNING "Failed to build benchmark: ${benchmark_VERSION_REQUIRED}")
        endif()
    endif()
endif()