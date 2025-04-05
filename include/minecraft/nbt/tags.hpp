#ifndef NBT_TAGS_HPP
#define NBT_TAGS_HPP

/**
  ==================================== NBT ====================================

  This file contains the description of NBT tags.
  Based on the MC wiki:
    https://minecraft.wiki/w/NBT_format

  Follow version 19133 of NBT format

  @author    Geoffrey Côte
  @date      05/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "minecraft/nbt/types.hpp"

namespace minecraft::nbt {

// ============================================================================
//    NBT Tags description
// ============================================================================

enum class Tags {
  END,
  Byte,
  Short,
  Int,
  Long,
  Float,
  Double,
  ByteArray,
  String,
  List,
  Compound,
  IntArray,
  LongArray
};

// ============================================================================
//    Tags conversion system
// ============================================================================

// Define conversion between type, int and enum
// -----------------------------------------------

template <typename T> constexpr uint8_t TagID() {
  static_assert(!std::is_base_of<NBTType, T>::value,
                "The type should be NBT type.");
  return 0;
}
template <typename T> constexpr Tags Tag() {
  static_assert(!std::is_base_of<NBTType, T>::value,
                "The type should be NBT type.");
  return Tags::END;
}
template <Tags T> constexpr uint8_t TagID() { return 0; }
template <uint8_t B> constexpr Tags Tag() { return Tags::END; }

#define NBT_TAG_FROM_TYPE(type, val)                                           \
  template <> inline constexpr uint8_t TagID<type>() { return val; }           \
  template <> inline constexpr uint8_t TagID<Tags::type>() { return val; }     \
  template <> inline constexpr Tags Tag<type>() { return Tags::type; }         \
  template <> inline constexpr Tags Tag<val>() { return Tags::type; }

// Implement them
// -----------------

NBT_TAG_FROM_TYPE(Byte, 1)
NBT_TAG_FROM_TYPE(Short, 2)
NBT_TAG_FROM_TYPE(Int, 3)
NBT_TAG_FROM_TYPE(Long, 4)
NBT_TAG_FROM_TYPE(Float, 5)
NBT_TAG_FROM_TYPE(Double, 6)
NBT_TAG_FROM_TYPE(ByteArray, 7)
NBT_TAG_FROM_TYPE(String, 8)
NBT_TAG_FROM_TYPE(List, 9)
NBT_TAG_FROM_TYPE(Compound, 10)
NBT_TAG_FROM_TYPE(IntArray, 11)
NBT_TAG_FROM_TYPE(LongArray, 12)

#undef NBT_TAG_FROM_TYPE

} // namespace minecraft::nbt

#endif