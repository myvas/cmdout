/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  cmdout_ext.h
 * @brief Helpers for myvas::cmdout.
 */
#pragma once

#include "cmdout.h"

#include <ostream>

namespace myvas {

/**
 * @brief Helper for printing a myvas::cmdout object.
 */
std::ostream &operator<<(std::ostream &os, const myvas::cmdout &value);

} // namespace myvas
