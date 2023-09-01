# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

###################################
# Find or fetch a CMake package
#
# Catch2 >=3.3.2
# Catch2::Catch2WithMain
# apt install catch2
###################################

find_package(Catch2 3.3.2 QUIET)
if(Catch2_FOUND)
    message(STATUS "Found Catch2: ${Catch2_VERSION}")
else()
    message(WARNING "CMake package Catch2 >=3.3.2 not found! Please wait for fetching from github.com...")

    include(FetchContent)
    FetchContent_Declare(Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(Catch2)
endif()