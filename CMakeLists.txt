# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.23)

###############################################################################
# NOTE:
# When you change the VERSION property, there are 3 things should be considered:
# (1) Obey the SemVer rules;
# (2) Add a new tag;
# (3) Sync the download url in `README.md`.
###############################################################################
project(cmdout_sln
	VERSION 0.1.14
)
set(CMDOUT_VERSION ${PROJECT_VERSION})

# Option: CMDOUT_MASTER_PROJECT
# ON  : if this project is being used directly
# OFF : if this project is being used via add_subdirectory
if(NOT DEFINED CMDOUT_MASTER_PROJECT)
	if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
		set(CMDOUT_MASTER_PROJECT ON)
	else()
		set(CMDOUT_MASTER_PROJECT OFF)
	endif()
endif()

# Basic settings
option(CMDOUT_BUILD_TESTS "Build tests" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_BUILD_EXAMPLES "Builds examples" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_BUILD_BENCHMARKS "Build benchmarks" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_INSTALL "Generate the install target" ${CMDOUT_MASTER_PROJECT})
mark_as_advanced(CMDOUT_BUILD_TESTS CMDOUT_BUILD_EXAMPLES CMDOUT_BUILD_BENCHMARKS)

# subprojects
add_subdirectory(cmdout)
if(CMDOUT_BUILD_TESTS)
	message(STATUS "Generating tests")
	enable_testing()
	add_subdirectory(tests)
endif()
if(CMDOUT_BUILD_EXAMPLES)
	message(STATUS "Generating examples")
	add_subdirectory(examples)
	cmdout_enable_warnings(example_myvas_system)
	cmdout_enable_warnings(example_std_system)
	cmdout_enable_warnings(example_myvas_cmdout)
endif()
if(CMDOUT_BUILD_BENCHMARKS)
	message(STATUS "Generating benchmarks")
	add_subdirectory(benchmarks)
endif()