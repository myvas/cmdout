# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Myvas Foundation

cmake_minimum_required(VERSION 3.27)

###############################################################################
# NOTE:
# When you change the VERSION property, there are 3 things should be considered:
# (1) Obey the SemVer rules;
# (2) Add a new tag;
# (3) Sync the download url in `README.md`.
###############################################################################
project(cmdout VERSION 0.2.0
    DESCRIPTION "a C++ library for executing a shell command and get its output."
    LANGUAGES CXX
)
set(CMDOUT_VERSION ${PROJECT_VERSION})
set(CMDOUT_SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

set(EXPORT_COMPILE_COMMANDS ON)

# include modules
find_package(Threads REQUIRED)
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
option(CMDOUT_BUILD_SHARED_LIBS "Build a shared library, otherwise a static library" OFF)
mark_as_advanced(CMDOUT_BUILD_TESTING
    CMDOUT_BUILD_EXAMPLES CMDOUT_BUILD_BENCHMARKS
)
message(STATUS "CMDOUT_INSTALL: ${CMDOUT_INSTALL}")
message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "CMDOUT_BUILD_SHARED_LIBS: ${CMDOUT_BUILD_SHARED_LIBS}")

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

################################
# Build Static/Shared library
################################
set(CMDOUT_SRCS
    "src/cmdout.cpp"
    "src/system.cpp"
    "src/cmdout_ext.cpp"
    "src/experimental/std_system_out.cpp"
)
if(CMDOUT_BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)
    if(WIN32)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)
        list(APPEND CMDOUT_SRCS ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
    endif()
    add_library(cmdout SHARED ${CMDOUT_SRCS})
else()
    add_library(cmdout STATIC ${CMDOUT_SRCS})
endif()
target_include_directories(cmdout PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_link_libraries(cmdout PUBLIC ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(cmdout PROPERTIES
    VERSION ${CMDOUT_VERSION}
    SOVERSION ${CMDOUT_SOVERSION}
)
add_library(myvas::cmdout ALIAS cmdout)

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

include(GNUInstallDirs)
if (CMDOUT_INSTALL)
    ###################
    # install docs
    ###################
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
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/"
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )

    #################
    # Install files
    #################
    # <prefix>/include/xxx
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    # <prefix>/lib/libxxx.a, libxxx.so
    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        # these get default values from GNUInstallDirs, no need to set them
        #RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
        #LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
        #ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
    )

    # pkg-config files: <prefix>/share/pkgconfig/xxx.pc
    message(STATUS "Generating pkg-config files")
    set(PKG_CONFIG_VERSION_STRING ${PROJECT_VERSION})
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
    get_target_property(PKG_CONFIG_CFLAGS ${PROJECT_NAME} INTERFACE_COMPILE_DEFINITIONS)
    if(PKG_CONFIG_CFLAGS)
        string(REPLACE ";" " -D" PKG_CONFIG_CFLAGS "${PKG_CONFIG_CFLAGS}")
        string(CONCAT PKG_CONFIG_CFLAGS "-D" "${PKG_CONFIG_CFLAGS}")
    else()
        set(PKG_CONFIG_CFLAGS "")
    endif()
    configure_file("cmake/${PROJECT_NAME}.pc.in" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)
    install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig"
    )

    # CMake files
    # /usr/lib/cmake/
    # Targets: xxxTargets.cmake
    # find_package in Config mode: xxxConfig.cmake, xxxConfigVersion.cmake
    export(TARGETS ${PROJECT_NAME}
        #NAMESPACE myvas::
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
    )
    install(EXPORT ${PROJECT_NAME}Targets
        #NAMESPACE myvas::
        FILE "${PROJECT_NAME}Targets.cmake"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}"
    )
    include(CMakePackageConfigHelpers)
    configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}"
    )
    write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}"
    )

    ######################################################
    # CPack
    ######################################################
    # options
    option(CAE_CPACK_BINARY_DEB "Pack a .deb file for deployment" ON)
    option(CAE_CPACK_BINARY_STGZ "Pack a .sh file for deployment" OFF)
    option(CAE_CPACK_BINARY_TGZ "Pack a .tar.gz file for deployment" OFF)
    option(CAE_CPACK_BINARY_TXZ "Pack a .tar.xz file for deployment" OFF)
    option(CAE_CPACK_SOURCE_TGZ "Pack a .tar.gz file for sourcecodes" OFF)
    option(CAE_CPACK_SOURCE_TXZ "Pack a .tar.gz file for sourcecodes" ON)

    # config and properties
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPackOptions.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/CPackOptions.cmake" @ONLY)
    set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackOptions.cmake")

    unset(CAE_CPACK_BINARY_GENERATOR_LIST)
    if(CAE_CPACK_BINARY_DEB)
        set(CPACK_BINARY_DEB "ON")
        list(APPEND CAE_CPACK_BINARY_GENERATOR_LIST "DEB")
    else()
        set(CPACK_BINARY_DEB "OFF")
    endif()
    if(CAE_CPACK_BINARY_STGZ)
        set(CPACK_BINARY_STGZ "ON")
        list(APPEND CAE_CPACK_BINARY_GENERATOR_LIST "STGZ")
    else()
        set(CPACK_BINARY_STGZ "OFF")
    endif()
    if(CAE_CPACK_BINARY_TGZ)
        set(CPACK_BINARY_TGZ "ON")
        list(APPEND CAE_CPACK_BINARY_GENERATOR_LIST "TGZ")
    else()
        set(CPACK_BINARY_TGZ "OFF")
    endif()
    if(CAE_CPACK_BINARY_TXZ)
        set(CPACK_BINARY_TXZ "ON")
        list(APPEND CAE_CPACK_BINARY_GENERATOR_LIST "TXZ")
    else()
        set(CPACK_BINARY_TXZ "OFF")
    endif()
    set(CPACK_BINARY_TZ "OFF")
    message(STATUS "CAE_CPACK_BINARY_GENERATOR_LIST: ${CAE_CPACK_BINARY_GENERATOR_LIST}")
    set(CPACK_GENERATOR ${CAE_CPACK_BINARY_GENERATOR_LIST})

    set(CPACK_SOURCE_IGNORE_FILES
        # default ignore files in CPack.cmake
        "/CVS/"
        "/\\\\.svn/"
        "/\\\\.bzr/"
        "/\\\\.hg/"
        "/\\\\.git/"
        "\\\\.swp$"
        "\\\\.#"
        "/#"

        # Files specific to version control.
        #"/\\\\.git/"
        "/\\\\.gitattributes$"
        "/\\\\.github/"
        "/\\\\.gitignore$"
        "/\\\\.hooks-config$"

        # Files produced by IDE.
        "/\\\\.vs/"
        "/\\\\.vscode/"
        "/out/"
        "/\\\\.build/"

        # Temporary files.
        "~$"

        # Others
        "/\\\\.readthedocs\\\\.yaml"
    )
    unset(CAE_CPACK_SOURCE_GENERATOR_LIST)
    if(CAE_CPACK_SOURCE_TGZ)
        set(CPACK_SOURCE_TGZ "ON")
        list(APPEND CAE_CPACK_SOURCE_GENERATOR_LIST "TGZ")
    else()
        set(CPACK_SOURCE_TGZ "OFF")
    endif()
    if(CAE_CPACK_SOURCE_TXZ)
        set(CPACK_SOURCE_TXZ "ON")
        list(APPEND CAE_CPACK_SOURCE_GENERATOR_LIST "TXZ")
    else()
        set(CPACK_SOURCE_TXZ "OFF")
    endif()
    set(CPACK_SOURCE_TZ "OFF")
    set(CPACK_SOURCE_TBZ2 "OFF")
    message(STATUS "CAE_CPACK_SOURCE_GENERATOR_LIST: ${CAE_CPACK_SOURCE_GENERATOR_LIST}")
    set(CPACK_SOURCE_GENERATOR ${CAE_CPACK_SOURCE_GENERATOR_LIST})

    include(CPack)
endif()
