# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

###################################
# Find or fetch a CMake package
#
# googletest >=1.10.0
# googletest
###################################

set(googletest_VERSION_REQUIRED 1.14.0)

find_package(googletest ${googletest_VERSION_REQUIRED} QUIET)
if(googletest_FOUND)
    message(STATUS "Found googletest: ${googletest_VERSION}")
else()
    message(STATUS "CMake package googletest ${googletest_VERSION_REQUIRED} or later not found!"
        " Please wait for fetching from https://github.com/google/googletest"
    )

    include(FetchContent)
    FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/refs/tags/v${googletest_VERSION_REQUIRED}.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        OVERRIDE_FIND_PACKAGE
    )
    find_package(googletest)
    if(googletest_FOUND)
        # TODO: We should extract `REAL` version number by hand, since there is no googletest_VERSION here!
        message(STATUS "Built googletest: ${googletest_VERSION}")
    else()
        message(FATAL_ERROR "Failed to build googletest: ${googletest_VERSION_REQUIRED}")
    endif()
endif()