/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  version.h
 * @brief Define sematic version number.
 *
 * @see `cmake/utils.cmake`.
 */
#pragma once

#define CMDOUT_VER_MAJOR 0
#define CMDOUT_VER_MINOR 1
#define CMDOUT_VER_PATCH 7

#define CMDOUT_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define CMDOUT_VERSION CMDOUT_TO_VERSION(CMDOUT_VER_MAJOR, CMDOUT_VER_MINOR, CMDOUT_VER_PATCH)