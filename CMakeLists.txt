# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25)

###############################################################################
# NOTE:
# When you change the VERSION property, there are 3 things should be considered:
# (1) Obey the SemVer rules;
# (2) Add a new tag;
# (3) Sync the download url in `README.md`.
###############################################################################
project(cmdout VERSION 0.1.23)
set(CMDOUT_VERSION ${PROJECT_VERSION})
set(CMDOUT_SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

# include modules
find_package(Threads)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(utils)

# CMDOUT_MASTER_PROJECT
# ON  : if this project is being used directly
# OFF : if this project is being used via add_subdirectory
if(NOT DEFINED CMDOUT_MASTER_PROJECT)
	if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
		set(CMDOUT_MASTER_PROJECT ON)
	else()
		set(CMDOUT_MASTER_PROJECT OFF)
	endif()
endif()
message(STATUS "CMDOUT_MASTER_PROJECT: ${CMDOUT_MASTER_PROJECT}")

###############################################################################
# options
###############################################################################
# Basic settings
option(CMDOUT_BUILD_TESTING "Build tests" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_BUILD_EXAMPLES "Builds examples" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_BUILD_BENCHMARKS "Build benchmarks" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_INSTALL "Generate intall rules for the cmdout library" ON)
option(CMDOUT_DOCS_DOXYGEN "Build documentation with Doxygen." ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_DOCS_INSTALL "Install the cmdout docs" ${CMDOUT_MASTER_PROJECT})
mark_as_advanced(CMDOUT_BUILD_TESTING CMDOUT_BUILD_EXAMPLES CMDOUT_BUILD_BENCHMARKS
	CMDOUT_DOCS_DOXYGEN CMDOUT_DOCS_INSTALL
	CMDOUT_INSTALL
)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMDOUT_BUILD_TESTING: ${CMDOUT_BUILD_TESTING}")
message(STATUS "CMDOUT_BUILD_EXAMPLES: ${CMDOUT_BUILD_EXAMPLES}")
message(STATUS "CMDOUT_BUILD_BENCHMARKS: ${CMDOUT_BUILD_BENCHMARKS}")
message(STATUS "CMDOUT_INSTALL: ${CMDOUT_INSTALL}")
message(STATUS "CMDOUT_DOCS_DOXYGEN: ${CMDOUT_DOCS_DOXYGEN}")
message(STATUS "CMDOUT_DOCS_INSTALL: ${CMDOUT_DOCS_INSTALL}")

# Advanced settings
option(CMDOUT_BUILD_SHARED "Build shared library" OFF)
option(CMDOUT_BUILD_PIC "Build position independent code (-fPIC)" ON)
option(CMDOUT_SANITIZE_ADDRESS "Enable address sanitizer in tests" OFF)
option(CMDOUT_BUILD_WARNINGS "Enable compiler warnings" OFF)
mark_as_advanced(CMDOUT_BUILD_SHARED CMDOUT_BUILD_PIC CMDOUT_SANITIZE_ADDRESS CMDOUT_BUILD_WARNINGS)
message(STATUS "CMDOUT_BUILD_SHARED: ${CMDOUT_BUILD_SHARED}")
message(STATUS "CMDOUT_BUILD_PIC: ${CMDOUT_BUILD_PIC}")
message(STATUS "CMDOUT_SANITIZE_ADDRESS: ${CMDOUT_SANITIZE_ADDRESS}")
message(STATUS "CMDOUT_BUILD_WARNINGS: ${CMDOUT_BUILD_WARNINGS}")

##############################
# main project: cmdout
##############################
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CYGWIN AND NOT MSYS AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL QNX)
	set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(CMDOUT_BUILD_PIC)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

################################
# Build Static/Shared library
################################
set(CMDOUT_SRCS
	"src/cmdout.cpp"
    "src/system.cpp"
    "src/cmdout_ext.cpp"
    "src/experimental/std_system_out.cpp"
)
if(CMDOUT_BUILD_SHARED OR BUILD_SHARED_LIBS)
	if(WIN32)
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)
		list(APPEND CMDOUT_SRCS ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
	endif()
	add_library(cmdout SHARED ${CMDOUT_SRCS} ${CMDOUT_ALL_HEADERS})
	target_compile_definitions(cmdout PUBLIC CMDOUT_SHARED_LIB)
else()
	add_library(cmdout STATIC ${CMDOUT_SRCS} ${CMDOUT_ALL_HEADERS})
endif()
add_library(myvas::cmdout ALIAS cmdout)

target_compile_definitions(cmdout PUBLIC CMDOUT_COMPILED_LIB)
set(CMDOUT_TARGET_INCLUDE_DIRECTORIES
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_include_directories(cmdout PUBLIC ${CMDOUT_TARGET_INCLUDE_DIRECTORIES})
message(STATUS "INCLUDE_DIRECTORIES: $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>")
target_link_libraries(cmdout PUBLIC ${CMAKE_THREAD_LIBS_INIT})
#set_target_properties(cmdout PROPERTIES COMPILE_FLAGS "-pthread" LINK_FLAGS "-pthread")
cmdout_enable_warnings(cmdout)

set_target_properties(cmdout PROPERTIES
	VERSION ${CMDOUT_VERSION}
	SOVERSION ${CMDOUT_SOVERSION}
)
if(CMDOUT_MASTER_PROJECT)
	set_target_properties(cmdout PROPERTIES DEBUG_POSTFIX d)
endif()

########################
# subprojects
########################
if(CMDOUT_BUILD_TESTING)
	message(STATUS "Generating tests")
    include(CTest)
	add_subdirectory(tests)
endif()
if(CMDOUT_BUILD_EXAMPLES)
	message(STATUS "Generating examples")
	add_subdirectory(examples)
endif()
if(CMDOUT_BUILD_BENCHMARKS)
	message(STATUS "Generating benchmarks")
	add_subdirectory(benchmarks)
endif()

###################
# install docs
###################
include(GNUInstallDirs)
if (CMDOUT_DOCS_DOXYGEN)
    # Require Doxygen 1.9.4
    #   apt install doxygen
    # Require GraphViz (dot) 2.43.0
    #   apt install graphviz
	# Require dot, treat the other components as optional
    find_package(Doxygen REQUIRED dot
        OPTIONAL_COMPONENTS mscgen dia
    )
    message(STATUS "Found Doxygen: ${DOXYGEN_VERSION}")
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    set(DOXYGEN_WARN_NO_PARAMDOC NO)
	set(DOXYGEN_QUIET YES)
	set(DOXYGEN_RECURSIVE YES)
	set(DOXYGEN_GENERATE_MAN NO)
	set(DOXYGEN_MARKDOWN_SUPPORT YES)

	set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DISABLE_INDEX NO)
    set(DOXYGEN_FULL_SIDEBAR YES)

	set(DOXYGEN_BINARY_TOC YES)
	set(DOXYGEN_TOC_EXPAND YES)

	set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
	set(DOXYGEN_EXTRACT_PACKAGE YES)
	set(DOXYGEN_EXTRACT_STATIC YES)
	set(DOXYGEN_SHOW_INCLUDE_FILES YES)

	set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "docs/index.md")
    set(DOXYGEN_EXCLUDE_PATTERNS "Doxyfile"
        " Doxyfile.in"
        " CMakeLists.txt"
        " */cmake/*"
        " .gitignore"
        " *.py"
        " requirements.txt"
    )
	doxygen_add_docs(cmdout_doxygen
		docs include examples/src ALL
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMENT "Building documentation with Doxygen."
	)

	if (CMDOUT_INSTALL AND CMDOUT_DOCS_INSTALL)
		install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/"
			DESTINATION ${CMAKE_INSTALL_DOCDIR}
		)
	endif()
else()
	if(CMDOUT_INSTALL AND CMDOUT_DOCS_INSTALL)
		install(DIRECTORY "${PROJECT_SOURCE_DIR}/docs/"
			DESTINATION ${CMAKE_INSTALL_DOCDIR}
		)
	endif()
endif()

##########################
# install cmdout library
##########################
include(GNUInstallDirs)
if(CMDOUT_INSTALL)
	message(STATUS "Generating install")

	set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/cmdoutConfig.cmake.in")
	set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/cmdoutConfig.cmake")
	set(config_targets_file "cmdoutConfigTargets.cmake")
	set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/cmdoutConfigVersion.cmake")
	set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/cmdout")
	set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
	set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")

	#################
	# Install files
	#################
	install(TARGETS cmdout
		EXPORT cmdout
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)

	###########################
	# Install pkg-config files
	###########################
	set(PKG_CONFIG_VERSION_STRING ${CMD_VERSION})
	if(IS_ABSOLUTE "${CMAKE_INSTALL_INCLUDEDIR}")
		set(PKG_CONFIG_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
	else()
		set(PKG_CONFIG_INCLUDEDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
	endif()
	if(IS_ABSOLUTE "${CMAKE_INSTALL_LIBDIR}")
		set(PKG_CONFIG_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
	else()
		set(PKG_CONFIG_LIBDIR "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
	endif()
	get_target_property(PKG_CONFIG_DEFINES cmdout INTERFACE_COMPILE_DEFINITIONS)
	string(REPLACE ";" " -D" PKG_CONFIG_DEFINES "${PKG_CONFIG_DEFINES}")
	string(CONCAT PKG_CONFIG_DEFINES "-D" "${PKG_CONFIG_DEFINES}")
	configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
	install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

	#############################
	# Install CMake config files
	#############################
	export(TARGETS cmdout NAMESPACE myvas:: FILE "${CMAKE_CURRENT_BINARY_DIR}/${config_targets_file}")
	install(EXPORT cmdout DESTINATION ${export_dest_dir} NAMESPACE myvas:: FILE ${config_targets_file})

	include(CMakePackageConfigHelpers)
	configure_package_config_file("${project_config_in}" "${project_config_out}" INSTALL_DESTINATION ${export_dest_dir})

	write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
	install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

	###########################################
	# Support creation of installable packages
	###########################################
	include(cmake/cmdoutCPack.cmake)
endif()
