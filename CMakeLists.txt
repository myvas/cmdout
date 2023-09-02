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
option(CMDOUT_INSTALL "Install the cmdout library" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_ENABLE_DOXYGEN "Build documentation with Doxygen." ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_DOCS_INSTALL "Install the cmdout docs" ${CMDOUT_MASTER_PROJECT})
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

###################
# install docs
###################
include(GNUInstallDirs)
if (CMDOUT_ENABLE_DOXYGEN)
	find_package(Doxygen REQUIRED)
	set(DOXYGEN_QUIET YES)
	set(DOXYGEN_RECURSIVE YES)
	set(DOXYGEN_GENERATE_HTML YES)
	set(DOXYGEN_GENERATE_MAN NO)
	set(DOXYGEN_MARKDOWN_SUPPORT YES)
	set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
	set(DOXYGEN_EXTRACT_PACKAGE YES)
	set(DOXYGEN_EXTRACT_STATIC YES)
	set(DOXYGEN_SHOW_INCLUDE_FILES YES)
	set(DOXYGEN_BINARY_TOC YES)
	set(DOXYGEN_TOC_EXPAND YES)
	set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "index.md")
	message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
	message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
	doxygen_add_docs(cmdout_doxygen
		docs cmdout/include ALL
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMENT "Building documentation with Doxygen."
	)

	if (CMDOUT_INSTALL AND CMDOUT_DOCS_INSTALL)
		# Current PROJECT_NAME is NOT cmdout, so we must concatenate a correct CMAKE_INSTALL_DOCDIR
		install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/"
			DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/cmdout
		)
	endif()
else()
	# Current PROJECT_NAME is NOT cmdout, so we must concatenate a correct CMAKE_INSTALL_DOCDIR
	if(CMDOUT_INSTALL AND CMDOUT_DOCS_INSTALL)
		install(DIRECTORY "${PROJECT_SOURCE_DIR}/docs/"
			DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/cmdout
		)
	endif()
endif()