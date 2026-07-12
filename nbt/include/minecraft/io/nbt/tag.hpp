// ============================================================================
// Project: SOLISMC-FILEIO
//
// NBT tags representation + conversion
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_TAG
#define SOLISMC_IO_NBT_TAG

#include "minecraft/io/nbt/errors.hpp"
#include <cstdint>

namespace minecraft::nbt {

// ============================================================================
enum class Tag : uint8_t {
#define X(name, value) name = value,
#include ".xmacros/x_nbt.hpp"
#undef X
};

// ============================================================================

/**
 * @brief Export a tag as a int
 */
constexpr int8_t to_int(const Tag tag) {
// Define X macro
#define X(name, value)                                                         \
  case Tag::name:                                                              \
    return value;

  switch (tag) {
#include ".xmacros/x_nbt.hpp"
  }
#undef X
}

// ============================================================================

/**
 * @brief Export a tag as a int
 */
constexpr Tag from_int(const uint8_t v) {
// Define X macro
#define X(name, value)                                                         \
  case value:                                                                  \
    return Tag::name;

  // Construct switch statement
  switch (v) {
#include ".xmacros/x_nbt.hpp"
  default:
    throw errors::InvalidNBTTag(v);
  }
#undef X
}

// ============================================================================

constexpr char *to_str(const Tag tag) {
// Define X macro
#define X(name, ...)                                                           \
  case Tag::name:                                                              \
    return #name;

  switch (tag) {
#include ".xmacros/x_nbt.hpp"
  }
#undef X
}

} // namespace minecraft::nbt

#endif