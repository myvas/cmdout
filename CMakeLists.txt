# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)
####################################
# SemVer major.minor.patch
# See: include/cmdout/version.h
####################################
include(cmake/utils.cmake)
cmdout_extract_version(CMDOUT_VERSION)
project(cmdout VERSION ${CMDOUT_VERSION} LANGUAGES CXX)
message(STATUS "Build ${PROJECT_NAME}: ${PROJECT_VERSION}")

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CYGWIN AND NOT MSYS AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL QNX)
	set(CMAKE_CXX_EXTENSIONS OFF)
endif()

include(CMakeDependentOption)
include(GNUInstallDirs)

###############################################################################
# CMDOUT_MASTER_PROJECT
# ON  : if this project is being used directly
# OFF : if this project is being used via add_subdirectory
###############################################################################
if(NOT DEFINED CMDOUT_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(CMDOUT_MASTER_PROJECT ON)
    else()
        set(CMDOUT_MASTER_PROJECT OFF)
    endif()
endif()

option(CMDOUT_BUILD_ALL "Build all artifacts" OFF)
option(CMDOUT_BUILD_EXAMPLES "Builds examples" ${CMDOUT_MASTER_PROJECT})
option(CMDOUT_BUILD_TESTS "Build tests" OFF)
option(CMDOUT_BUILD_BENCHMARKS "Build benchmarks (Requires https://github.com/google/benchmark.git to be installed)" OFF)
option(CMDOUT_BUILD_SHARED "Build shared library" OFF)
option(CMDOUT_BUILD_PIC "Build position independent code (-fPIC)" OFF)
option(CMDOUT_SANITIZE_ADDRESS "Enable address sanitizer in tests" OFF)
option(CMDOUT_BUILD_WARNINGS "Enable compiler warnings" OFF)
option(CMDOUT_INSTALL "Generate the install target" ${CMDOUT_MASTER_PROJECT})

if(CMDOUT_BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

find_package(Threads)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
#############################
# cmdout sources
#############################
set(CMDOUT_SRCS
    "src/cmdout.cpp" "src/system.cpp"
)
################################
# Build Static/Shared library
################################
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
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/cmdout>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cmdout>"
)
target_include_directories(cmdout PUBLIC ${CMDOUT_TARGET_INCLUDE_DIRECTORIES})
message(STATUS "INCLUDE_DIRECTORIES: $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/cmdout>")
target_link_libraries(cmdout PUBLIC ${CMAKE_THREAD_LIBS_INIT})
#set_target_properties(cmdout PROPERTIES COMPILE_FLAGS "-pthread" LINK_FLAGS "-pthread")
cmdout_enable_warnings(cmdout)

set_target_properties(cmdout PROPERTIES
    VERSION ${CMDOUT_VERSION}
    SOVERSION ${CMDOUT_VERSION_MAJOR}.${CMDOUT_VERSION_MINOR}
)
if(CMDOUT_MASTER_PROJECT)
    set_target_properties(cmdout PROPERTIES DEBUG_POSTFIX d)
endif()

##################
# /examples
##################
if(CMDOUT_BUILD_EXAMPLES OR CMDOUT_BUILD_ALL)
    message(STATUS "Generating examples")
	add_subdirectory(examples)
    cmdout_enable_warnings(examples)
endif()

##################
# /tests
##################
if(CMDOUT_BUILD_TESTS OR CMDOUT_BUILD_ALL)
    message(STATUS "Generating tests")
    enable_testing()
    add_subdirectory(tests)
endif()

##################
# /benchmarks
##################
if(CMDOUT_BUILD_BENCHMARKS OR CMDOUT_BUILD_ALL)
    message(STATUS "Generating benchmarks")
    add_subdirectory(benchmarks)
endif()

##################
# install
##################
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


# Hide advanced options from normal settings view
mark_as_advanced(
    #CMDOUT_BUILD_ALL
    CMDOUT_BUILD_TESTS CMDOUT_BUILD_EXAMPLES CMDOUT_BUILD_BENCHMARKS
    #CMDOUT_BUILD_SHARED CMDOUT_BUILD_PIC CMDOUT_BUILD_WARNINGS
    #CMDOUT_SANITIZE_ADDRESS CMDOUT_COMPILED_LIB
)
