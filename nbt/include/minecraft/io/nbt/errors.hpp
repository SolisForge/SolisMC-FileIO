// ============================================================================
// Project: SOLISMC-FILEIO
//
// Error for NBT related codes
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_ERRORS
#define SOLISMC_NBT_ERRORS
#include "solis/utils/errors.hpp"

namespace minecraft::nbt::errors {

struct NBTError : solis::SolisError {

  NBTError(const std::string &&msg) : solis::SolisError(msg) {}
};

// ============================================================================
// NBT Tags-related errors
// ============================================================================
struct InvalidNBTTag : NBTError {
  sconstchar MSG{"Tag #{:d} does not exist (in range [0-12])"};

  InvalidNBTTag(const uint8_t tag_value)
      : NBTError(fmt::format(MSG, tag_value)) {}
};

} // namespace minecraft::nbt::errors

#endif