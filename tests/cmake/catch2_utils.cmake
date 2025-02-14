# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Myvas Foundation

###############################################################################
# test utils
###############################################################################

# add a test target with sources and dependencies.
# Usage:
# cmdout_add_catch2(target source1 [source2...]
#           [DEPENDS lib1 [lib2...]]
# )
# NOTE:
# DEPENDS Catch2::Catch2WithMain will be always added.
function(cmdout_add_catch2 test_target)
	set(options) # N/A
	set(oneValueArgs) # N/A
	set(multiValueArgs DEPENDS)
	cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(NOT arg_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "cmdout_add_test: requires at least one source file")
	endif()

	add_executable(${test_target} ${arg_UNPARSED_ARGUMENTS})
	cmdout_enable_warnings(${test_target})
	if(arg_DEPENDS)
		target_link_libraries(${test_target} PRIVATE ${arg_DEPENDS})
	endif()
	target_link_libraries(${test_target} PRIVATE Catch2::Catch2WithMain)
	if(CMDOUT_SANITIZE_ADDRESS)
		cmdout_enable_sanitizer(${test_target})
	endif()
	add_test(NAME ${test_target} COMMAND ${test_target})
	set_tests_properties(${test_target} PROPERTIES RUN_SERIAL ON)
endfunction()
