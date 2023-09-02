/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  cmdout.hpp
 * @brief This class myvas::cmdout and function myvas::system() is used to
 *        execute a shell command and get its output, within timeout feature.
 *
 * @see {std::system()}
 */
#pragma once

#include <string>
#include <chrono>

namespace myvas {

const int64_t CMDOUT_TIMEOUT_MILLISECONDS = 10 * 1000;

/**
 * @brief Executor to execute a shell command and get its output.
 */
class cmdout
{
	std::string cmd_= "exit 0";
	int64_t timeout_ms_ = CMDOUT_TIMEOUT_MILLISECONDS;
	int status_ = EXIT_FAILURE;
	std::string out_ = "Failed";

public:

	/**
	 * @brief Gets the exit code after exec returned.
	 *
	 * The value is one of the following:
	 * - If cmd is empty, then a nonzero value if a shell is available, or 0 if
	 * no shell is available.
	 * - If a child process in cmd could not be created, or its status could not
	 * be retrieved, the return value is -1 and errno is set to indicate the
	 * error.
	 * - If a shell could not be executed, then the return value is 127.
	 * - If all system call succeed, then the return value is the status of the
	 * last command in cmd.
	 */
	int status() const;
	cmdout& status(int);

	/**
	 * @brief Gets the content of stdout (file descriptor 1).
	 */
	std::string out() const;
	cmdout& out(const std::string&);

	/**
	 * @brief Gets the shell command line.
	 */
	std::string cmd() const;
	cmdout& cmd(const std::string&);

	std::chrono::milliseconds timeout() const;
	int64_t timeout_ms() const;
	cmdout& timeout(std::chrono::milliseconds);
	cmdout& timeout(int64_t);

	/**
	 * @brief Default to cmdout("exit 0").
	 */
	cmdout();

	/**
	 * @brief Execute a shell command specified in cmd, waiting for the result until specified timeout (10 seconds) has elapsed.
	 * NOTE: A shell command with a tail of `2>&1` will catch both stdout and stderr, otherwise stdout only.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string
	 * containing a shell command.
	 *
	 * @return The return value is a cmdout object, in which wraps the status code
	 * and output string of the shell command.
	 */
	explicit cmdout(const std::string& cmd);

	/**
	 * @brief Execute a shell command specified in cmd, waiting for the result until specified timeout has elapsed.
	 * NOTE: A shell command with a tail of `2>&1` will catch both stdout and
	 * stderr, otherwise stdout only.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string
	 * containing a shell command.
	 *
	 * @param timeout_ms timeout in milliseconds. (If zero, defaults to 10 seconds)
	 *
	 * @return The return value is a cmdout object, in which wraps the status code
	 * and output string of the shell command.
	 */
	explicit cmdout(const std::string& cmd, int64_t timeout_ms);

	/**
	 * @brief Execute a shell command specified in cmd, waiting for the result until specified timeout has elapsed.
	 * NOTE: A shell command with a tail of `2>&1` will catch both stdout and stderr, otherwise stdout only.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string
	 * containing a shell command.
	 *
	 * @param timeout timeout duration. (If zero, defaults to 10 seconds)
	 *
	 * @return The return value is a cmdout object, in which wraps the status code
	 * and output string of the shell command.
	 */
	explicit cmdout(const std::string& cmd, std::chrono::milliseconds timeout);

	/**
	 * @brief Constructs an output.
	 *
	 * @param status EXIT_SUCCESS, EXIT_FAILURE or errno
	 * @see cerrno and cstdlib.
	 */
	explicit cmdout(const std::string& cmd, std::chrono::milliseconds timeout, int status, const std::string& out);
	explicit cmdout(const std::string& cmd, int64_t timeout_ms, int status, const std::string& out);

	cmdout(const cmdout&);
	cmdout& operator=(const cmdout&);
	cmdout(cmdout&&);
	cmdout& operator=(cmdout&&);

	virtual ~cmdout();

	/**
	 * @brief Execute a specified shell command with arguments initialized in this class.
	 */
	cmdout& exec();
};

/**
 * @brief Execute a shell command specified in cmd.
 *
 * @return Returns a cmdout object (without timeout)
 */
cmdout system(const std::string& cmd);

/**
 * @brief Execute a shell command specified in cmd and get its output before time runs out.
 *
 * @param timeout timeout duration, defaults to 10 seconds
 *
 * @return Returns a cmdout object
 */
cmdout system_timeout(const std::string& cmd, std::chrono::milliseconds timeout);

/**
 * @brief Execute a shell command specified in cmd and get its output before time runs out.
 *
 * @param timeout_ms timeout in milliseconds, defaults to 10 seconds
 *
 * @return Returns a cmdout object
 */
cmdout system_timeout_ms(const std::string& cmd, int64_t timeout_ms);

} // namespace myvas