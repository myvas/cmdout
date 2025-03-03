/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  cmdout.h
 * @brief Class myvas::cmdout is used to execute a shell command and get its output, within timeout feature.
 * And function myvas::system() performs the executing logic.
 *
 * @see std::system() - The standard implementation in POSIX system.
 */
#pragma once

#include <string>
#include <chrono>

namespace myvas {

/**
 * @brief Default timeout 10 seconds.
 */
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
	 * @remarks
	 * The values defined in cstdlib:
	 * - EXIT_SUCCESS - The shell command goes well and returns successfully.
	 * - EXIT_FAILURE - The shell command executed and exit with 1.
	 *
	 * The values defined in cerrno:
	 * - ETIME - The shell command does NOT return before time runs out.
	 * - ENOENT - The shell command executed and exit with error "No such file or directory"
	 * - ... - Please read the <stdlib.h> to get more possible values
	 *
	 * @return This value is one of the following:
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

	/**
	 * @brief Sets the status value.
	 */
	cmdout& status(int);

	/**
	 * @brief Gets the content of stdout (file descriptor 1).
	 *
	 * A string came from the `stdout`, i.e. the file descriptor 1 in POSIX system.
	 * If you also want to get the `stderr`, append ` 2>&1` would be an answer in POSIX system.
	 */
	std::string out() const;
	/**
	 * @brief Sets the out string.
	 */
	cmdout& out(const std::string&);

	/**
	 * @brief Gets the shell command line.
	 */
	std::string cmd() const;
	/**
	 * @brief Sets the cmd string.
	 */
	cmdout& cmd(const std::string&);

	/**
	 * @brief Gets the timeout duration in milliseconds.
	 */
	std::chrono::milliseconds timeout() const;
	/**
	 * @brief Gets the timeout in milliseconds.
	 */
	int64_t timeout_ms() const;
	/**
	 * @brief Sets the timeout.
	 */
	cmdout& timeout(std::chrono::milliseconds);
	/**
	 * @brief Sets the timeout value.
	 */
	cmdout& timeout(int64_t);

	/**
	 * @brief Default to myvas::cmdout("exit 0").
	 */
	cmdout();

	/**
	 * @brief Execute a shell command specified in cmd, waiting for the result until specified timeout (10 seconds) has elapsed.
	 * @remarks A shell command with a tail of `2>&1` will catch both stdout and stderr, otherwise stdout only.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string
	 * containing a shell command.
	 */
	explicit cmdout(const std::string& cmd);

	/**
	 * @brief Execute a shell command specified in cmd, waiting for the result until specified timeout has elapsed.
	 * @remarks A shell command with a tail of `2>&1` will catch both stdout and
	 * stderr, otherwise stdout only.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string
	 * containing a shell command.
	 *
	 * @param timeout_ms timeout in milliseconds. (If zero, defaults to 10 seconds)
	 */
	explicit cmdout(const std::string& cmd, int64_t timeout_ms);

	/**
	 * @brief Execute a shell command specified in cmd, waiting for the result until specified timeout has elapsed.
	 * @remarks A shell command with a tail of `2>&1` will catch both stdout and stderr, otherwise stdout only.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string
	 * containing a shell command.
	 *
	 * @param timeout timeout duration. (If zero, defaults to 10 seconds)
	 */
	explicit cmdout(const std::string& cmd, std::chrono::milliseconds timeout);

	/**
	 * @brief Constructs an output.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string containing a shell command.
	 * @param timeout timeout duration. (If zero, defaults to 10 seconds)
	 * @param status EXIT_SUCCESS, EXIT_FAILURE or errno
	 * @param out output string
	 */
	explicit cmdout(const std::string& cmd, std::chrono::milliseconds timeout, int status, const std::string& out);
	/**
	 * @brief Constructs an output.
	 *
	 * @param cmd The cmd argument is a pointer to a null-terminated string containing a shell command.
	 * @param timeout_ms timeout. (If zero, defaults to 10 seconds)
	 * @param status EXIT_SUCCESS, EXIT_FAILURE or errno
	 * @param out output string
	 */
	explicit cmdout(const std::string& cmd, int64_t timeout_ms, int status, const std::string& out);

	/**
	 * @brief Copy constructor.
	 */
	cmdout(const cmdout&);
	/**
	 * @brief Copy assignment.
	 */
	cmdout& operator=(const cmdout&);
	/**
	 * @brief Move constructor.
	 */
	cmdout(cmdout&&);
	/**
	 * @brief Move assignment.
	 */
	cmdout& operator=(cmdout&&);

	/**
	 * @brief Destructor.
	 */
	virtual ~cmdout();

	/**
	 * @brief Execute a specified shell command with arguments initialized in this class.
	 *
	 * @return The return value is a myvas::cmdout reference, in which wraps the status code
	 * and output string of the shell command.
	 */
	cmdout& exec();
};

/**
 * @brief Execute a shell command specified in cmd.
 *
 * @param cmd The cmd argument is a pointer to a null-terminated string containing a shell command.
 *
 * @return Returns a myvas::cmdout object (without timeout)
 */
cmdout system(const std::string& cmd);

/**
 * @brief Execute a shell command specified in cmd and get its output before time runs out.
 *
 * @param cmd The cmd argument is a pointer to a null-terminated string containing a shell command.
 * @param timeout timeout duration, defaults to 10 seconds
 *
 * @return Returns a myvas::cmdout object
 */
cmdout system_timeout(const std::string& cmd, std::chrono::milliseconds timeout);

/**
 * @brief Execute a shell command specified in cmd and get its output before time runs out.
 *
 * @param cmd The cmd argument is a pointer to a null-terminated string containing a shell command.
 * @param timeout_ms timeout in milliseconds, defaults to 10 seconds
 *
 * @return Returns a myvas::cmdout object
 */
cmdout system_timeout_ms(const std::string& cmd, int64_t timeout_ms);

} // namespace myvas
