# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

###############################################################################
# Find or fetch a CMake package: Catch2 >=3.3.2
###############################################################################

set(Catch2_VERSION_REQUIRED 3.3.2)
find_package(Catch2 ${Catch2_VERSION_REQUIRED} QUIET)
if(Catch2_FOUND)
    message(STATUS "Found Catch2: ${Catch2_VERSION}")
else()
    set(Catch2_DOWNLOAD_URL https://github.com/catchorg/Catch2/archive/refs/tags/v${Catch2_VERSION_REQUIRED}.tar.gz)
    message(STATUS "CMake package Catch2 ${Catch2_VERSION_REQUIRED} or later not found!"
        " Please wait for fetching from ${Catch2_DOWNLOAD_URL}"
    )

    include(FetchContent)
    FetchContent_Declare(Catch2
        URL ${Catch2_DOWNLOAD_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        OVERRIDE_FIND_PACKAGE
    )
    find_package(Catch2)
    if(Catch2_FOUND)
        # TODO: We should extract `REAL` version number by hand, since there is no Catch2_VERSION here!
        message(STATUS "Built Catch2: ${Catch2_VERSION}")
    else()
        message(FATAL_ERROR "Failed to build Catch2: ${Catch2_VERSION_REQUIRED}")
    endif()
endif()