// ============================================================================
// Project: SOLISMC_IO
//
// File reading possibilities
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once

#include <filesystem>
#include <string>

/**
 * @brief Read bytes from stdin
 *
 * @return std::string
 */
std::string from_stdin();

/**
 * @brief Read bytes from stdin
 *
 * @return std::string
 */
std::string from_file(std::filesystem::path const &);