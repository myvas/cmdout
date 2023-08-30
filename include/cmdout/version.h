/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  version.h
 * @brief Define sematic version number.
 *
 * NOTE: If you change the version number, there are 3 things should be considered:
 * (1) Obey the SemVer rules;
 * (2) Add a new tag;
 * (3) Sync the download url in `README.md`.
 *
 * @see `cmake/utils.cmake`
 */
#pragma once

/**
 * @brief When you make incompatible API changes, You'd increment the MAJOR version.
 */
#define CMDOUT_VERSION_MAJOR 0
/**
 * @brief When you add functionality in a backward compatible manner, You'd increment the MINOR version.
 */
#define CMDOUT_VERSION_MINOR 1
/**
 * @brief When you make backward compatible bug fixes, You'd increment the PATCH version.
 */
#define CMDOUT_VERSION_PATCH 9

#define CMDOUT_TO_VERSION_NUMBER(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define CMDOUT_VERSION_NUMBER CMDOUT_TO_VERSION_NUMBER(CMDOUT_VERSION_MAJOR, CMDOUT_VERSION_MINOR, CMDOUT_VERSION_PATCH)