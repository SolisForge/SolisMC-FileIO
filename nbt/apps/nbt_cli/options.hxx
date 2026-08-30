// ============================================================================
// Project: SOLISMC_IO
//
// Arguments for NBT cli app
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once

#include "minecraft/io/nbt/tags.hxx"
#include <filesystem>

using minecraft::nbt::Tags;

// ============================================================================
// Helper structs
// ============================================================================
enum class ReadFrom { FILE, STDIN };

// ============================================================================
// Options struct
// ============================================================================
/**
 * @brief Arguments for the NBT cli application
 */
struct Options {

  ReadFrom input = ReadFrom::STDIN; //< Where to read the bytes from
  std::filesystem::path input_file{};

  Tags data_type = Tags::COMPOUND; //< Which type to parse from the bytes
};

// ============================================================================
/**
 * @brief Parse the command line arguments
 *
 * @param argc number of arguments given
 * @param argv arguments
 * @return constructed options structure
 */
Options parse_args(int argc, char **argv);
