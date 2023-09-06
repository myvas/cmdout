/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  cmdout_ext.hpp
 * @brief Helpers for myvas::cmdout.
 */
#pragma once

#include "cmdout.hpp"

#include <ostream>

namespace myvas {

/**
 * @brief Helper for printing a myvas::cmdout object.
 */
std::ostream &operator<<(std::ostream &os, const myvas::cmdout &value);

} // namespace myvas
