/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  cmdout.cpp
 * @brief Extensions for class myvas::cmdout.
 */
#include <cmdout/cmdout_ext.hpp>

namespace myvas {

std::ostream &operator<<(std::ostream &os, const myvas::cmdout &value)
{
    os << "cmdout: " << value.cmd() << "\n(return " << value.status() << " length " << value.out().length() << ")\n"
       << std::string(80, '-') << "\n"
       << value.out();
    return os;
}

} // namespace myvas
