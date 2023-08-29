cmake_minimum_required(VERSION 3.19)
project(cmdout VERSION 0.1.4)

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

option(CMDOUT_DEMO_BUILD "Builds the demo subproject" ON)
option(CMDOUT_INSTALL "Enable installation of cmdout. (Projects embedding cmdout may want to turn this OFF.)" ON)

add_subdirectory(cmdout)
if(CMDOUT_DEMO_BUILD)
	add_subdirectory(demo)
endif()
