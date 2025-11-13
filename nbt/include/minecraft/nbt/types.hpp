// ============================================================================
// Project: SOLISMC_FILEIO
//
// Definition of the NBT tags types.
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/11/2025 (created 13/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_TYPES_HPP
#define SOLISMC_NBT_TYPES_HPP

#include <cstdint>

namespace minecraft {

typedef uint8_t TagID_t;

/**
 * @brief NBT tags byte values
 */
struct NBTTags {
  enum Tags : TagID_t {
    // Primitives
    Byte = 1,
    Short = 2,
    Int = 3,
    Long = 4,
    Float = 5,
    Double = 6,
    String = 8,

    // Array
    ByteArray = 7,
    List = 9,
    IntArray = 11,
    LongArray = 12,

    // Special
    Compound = 10,
    END = 0,
    ERROR = 255
  };

  // --------------------------------------------------------------------------

  /**
   * @brief Get the name of the given tag as a const char*
   *
   * @param tag the NBT tag enumeration
   * @return the name of the tag
   */
  static constexpr const char *getName(NBTTags::Tags tag);
};

// ============================================================================

} // namespace minecraft

#endif