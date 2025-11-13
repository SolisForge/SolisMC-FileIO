#ifndef NBT_TAGS_HPP
#define NBT_TAGS_HPP

/**
  ==================================== NBT ====================================

  This file contains the description of NBT tags.
  Based on the MC wiki:
    https://minecraft.wiki/w/NBT_format

  Follow version 19133 of NBT format

  @author    Meltwin
  @date      05/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "minecraft/nbt/format/types.hpp"
#include "solis/utils/static.hpp"

namespace minecraft::nbt {

// ============================================================================
//    NBT Tags decription
// ============================================================================

typedef uint8_t TagID_t;

/**
 * @brief Enumeration correspondance for the NBT types.
 */
struct NBTTags {
  enum Tags : TagID_t {
    END = 0,
    Byte = 1,
    Short = 2,
    Int = 3,
    Long = 4,
    Float = 5,
    Double = 6,
    ByteArray = 7,
    String = 8,
    List = 9,
    Compound = 10,
    IntArray = 11,
    LongArray = 12,

    ERROR = 255
  };
};

} // namespace minecraft::nbt

#endif