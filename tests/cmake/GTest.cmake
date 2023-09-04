# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

###############################################################################
# Find or fetch a CMake package: GTest >=1.14.0
###############################################################################
set(GTest_VERSION_REQUIRED 1.14.0)
find_package(GTest ${googletest_VERSION_REQUIRED} QUIET)
if(GTest_FOUND)
    message(STATUS "Found GTest: ${GTest_VERSION}")
else()
    set(GTest_DOWNLOAD_URL https://github.com/google/googletest/archive/refs/tags/v${GTest_VERSION_REQUIRED}.tar.gz)
    message(STATUS "CMake package GTest ${GTest_VERSION_REQUIRED} or later not found!"
        " Please wait for fetching from ${GTest_DOWNLOAD_URL}"
    )

    include(FetchContent)
    set(INSTALL_GTEST OFF CACHE INTERNAL "")
    FetchContent_Declare(GTest
        URL ${GTest_DOWNLOAD_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        OVERRIDE_FIND_PACKAGE
    )
    find_package(GTest)
    if(GTest_FOUND)
        # TODO: We should extract `REAL` version number by hand, since there is no GTest_VERSION here!
        message(STATUS "Built GTest: ${GTest_VERSION}")
    else()
        message(FATAL_ERROR "Failed to build GTest: ${GTest_VERSION_REQUIRED}")
    endif()
endif()
