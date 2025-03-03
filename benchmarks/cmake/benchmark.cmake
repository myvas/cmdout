# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Myvas Foundation

###############################################################################
# Find or fetch a CMake package: benchmark >=1.8.3
###############################################################################

set(benchmark_VERSION_REQUIRED 1.8.3)
if(TARGET benchmark::benchmark)
    get_target_property(benchmark_VERSION benchmark::benchmark VERSION)
    if(benchmark_VERSION VERSION_GREATER_EQUAL benchmark_VERSION_REQUIRED)
        message(STATUS "Found benchmark: ${benchmark_VERSION}")
    else()
        message(FATAL_ERROR "Found benchmark: ${benchmark_VERSION}, but we need ${benchmark_VERSION_REQUIRED} or leater!")
    endif()
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
