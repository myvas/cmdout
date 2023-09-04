# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

###############################################################################
# Find or fetch a CMake package: benchmark >=1.8.3
###############################################################################

set(benchmark_VERSION_REQUIRED 1.8.3)
find_package(benchmark ${benchmark_VERSION_REQUIRED} QUIET)
if(benchmark_FOUND)
    message(STATUS "Found benchmark: ${benchmark_VERSION}")
else()
    set(benchmark_DOWNLOAD_URL https://github.com/google/benchmark/archive/refs/tags/v${benchmark_VERSION_REQUIRED}.tar.gz)
    message(STATUS "CMake package benchmark ${benchmark_VERSION_REQUIRED} or later not found!"
        " Please wait for fetching from ${benchmark_DOWNLOAD_URL}"
    )

    include(FetchContent)
    ###############################################
    # Do not build and run googlebenchmark tests!
    ###############################################
    set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
    set(BENCHMARK_INSTALL_DOCS OFF CACHE INTERNAL "")
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE INTERNAL "")
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE INTERNAL "")
    set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE INTERNAL "")
    FetchContent_Declare(benchmark
        URL ${benchmark_DOWNLOAD_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(benchmark)
endif()
