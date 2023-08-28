/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  cmdout.hpp
 * @brief `myvas::system()` is used to invoke a shell command and get its output, within timeout feature.
 *
 * @see {std::system()}
 */
#pragma once

#include <string>
#include <string_view>
#include <chrono>
#include <cstdlib>
#include <cerrno>

namespace myvas {

/**
 * @brief Executor to execute a shell command and get its output.
 */
class cmdout
{
	int status_;
	std::string out_;
	std::string cmd_;

public:
	/**
	 * @brief Gets the exit code after `exec` returned.
	 *
	 * The value is one of the following:
	 * - If `cmd` is empty, then a nonzero value if a shell is available, or 0 if
	 * no shell is available.
	 * - If a child process in `cmd` could not be created, or its status could not
	 * be retrieved, the return value is -1 and `errno` is set to indicate the
	 * error.
	 * - If a shell could not be executed, then the return value is 127.
	 * - If all system call succeed, then the return value is the status of the
	 * last command in `cmd`.
	 */
	int status() const;

	void status(int value);

	/**
	 * @brief Gets the content of `stdout` (file descriptor 1).
	 */
	std::string_view out() const;

	void out(std::string_view value);

	/**
	 * @brief Gets the shell command line.
	 */
	std::string_view cmd() const;

	void cmd(std::string_view value);

	/**
	 * @brief Default to cmdout(ENOTSUP, "Not supported").
	 */
	cmdout();

	cmdout(std::string_view cmd);

	/**
	 * @brief Constructs an output.
	 *
	 * @param [in] status EXIT_SUCCESS, EXIT_FAILURE or errno
	 * @see `cerrno` and `cstdlib`.
	 */
	explicit cmdout(int status, std::string_view out, std::string_view cmd);

	cmdout(const cmdout&);
	cmdout& operator=(const cmdout&);
	cmdout(cmdout&&);
	cmdout& operator=(cmdout&&);

	virtual ~cmdout();
};

/**
 * @brief Execute a shell command specified in `cmd`, waiting for the result until specified timeout (10 seconds) has elapsed.
 * NOTE: A shell command with a tail of `2>&1` will catch both stdout and stderr, otherwise stdout only.
 *
 * @param [in] cmd The `cmd` argument is a pointer to a null-terminated string
 * containing a shell command.
 *
 * @return The return value is a `cmdout` object, in which wraps the status code
 * and output string of the shell command.
 */
cmdout system(const char* cmd);

/**
 * @brief Execute a shell command specified in `cmd`, waiting for the result until specified timeout has elapsed.
 * NOTE: A shell command with a tail of `2>&1` will catch both stdout and
 * stderr, otherwise stdout only.
 *
 * @param [in] cmd The `cmd` argument is a pointer to a null-terminated string
 * containing a shell command.
 *
 * @param [in] timeout_ms timeout in milliseconds. (If zero, defaults to 10 seconds)
 *
 * @return The return value is a `cmdout` object, in which wraps the status code
 * and output string of the shell command.
 */
cmdout system(const char* cmd, unsigned timeout_ms);

/**
 * @brief Execute a shell command specified in `cmd`, waiting for the result until specified timeout has elapsed.
 * NOTE: A shell command with a tail of `2>&1` will catch both stdout and stderr, otherwise stdout only.
 *
 * @param [in] cmd The `cmd` argument is a pointer to a null-terminated string
 * containing a shell command.
 *
 * @param [in] timeout_ms timeout in milliseconds. (If zero, defaults to 10 seconds)
 *
 * @return The return value is a `cmdout` object, in which wraps the status code
 * and output string of the shell command.
 */
cmdout system(const char* cmd, std::chrono::milliseconds timeout_ms);

} // namespace myvas