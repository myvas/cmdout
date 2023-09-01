/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  cmdout_ext.hpp
 * @brief Helpers for `myvas::cmdout`.
 */
#pragma once

#include "cmdout.hpp"

#include <ostream>

 /**
  * @brief Helper for printing a cmdout object.
  */
std::ostream& operator<<(std::ostream& os, const myvas::cmdout& value)
{
	os << "cmdout: " << value.cmd() << "\n(return " << value.status()
		<< " length " << value.out().length() << ")\n"
		<< std::string(80, '-') << "\n" << value.out();
	return os;
}