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

namespace minecraft::nbt {

// ============================================================================
//    NBT Tags decription
// ============================================================================

/**
 * @brief Enumeration correspondance for the NBT types.
 */
enum class NBTTags : uint8_t {
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

// Define conversion between type, int, enum and str
// --------------------------------------------------

/**
 * @brief Get the tag value for the given NBT type.
 *
 * @tparam T the type of interest
 * @return the value of the tag
 */
template <typename T> constexpr uint8_t TagID() {
  static_assert(!std::is_base_of<NBTType, T>::value,
                "The type should be NBT type.");
  return 0;
}
/**
 * @brief Get the tag value for the given NBT type enumeration.
 *
 * @tparam T the enumeration
 * @return the value of the tag
 */
template <NBTTags T> constexpr uint8_t TagID() { return 0; }

// ----------------------------------------------------------------------------

/**
 * @brief Get the tag enum for the given NBT type.
 *
 * @tparam T the type of interest
 * @return the corresponding enum
 */
template <typename T> constexpr NBTTags Tag() {
  static_assert(!std::is_base_of<NBTType, T>::value,
                "The type should be NBT type.");
  return NBTTags::END;
}
/**
 * @brief Get the tag enum for the given NBT type's ID.
 *
 * @tparam B the ID of the type
 * @return the corresponding enum
 */
template <uint8_t B> constexpr NBTTags Tag() { return NBTTags::END; }

// ----------------------------------------------------------------------------

/**
 * @brief Get the tag str version for the given type's enum
 *
 * @tparam T the nbt type enum
 * @return the corresponding string representation
 */
template <NBTTags T> constexpr const char *TagStr() { return "END"; }

/**
 * @brief Get the tag str version for the given type
 *
 * @tparam T the nbt type
 * @return the corresponding string representation
 */
template <typename T> constexpr const char *TagStr() { return "END"; }

/**
 * @brief Get the tag str version for the given type's id
 *
 * @tparam B the nbt type id
 * @return the corresponding string representation
 */
template <uint8_t B> constexpr const char *TagStr() { return "END"; }

// ----------------------------------------------------------------------------

/*!
 * \def NBT_TAG_FROM_TYPE(type, val)
 * Register a NBT type - Type Enum - Type ID correspondance by making
 * specialization of conversion functions.
 */
#define NBT_TAG_FROM_TYPE(type, val)                                           \
  template <> inline constexpr uint8_t TagID<type>() { return val; }           \
  template <> inline constexpr uint8_t TagID<NBTTags::type>() { return val; }  \
  template <> inline constexpr NBTTags Tag<type>() { return NBTTags::type; }   \
  template <> inline constexpr NBTTags Tag<val>() { return NBTTags::type; }    \
  template <> inline constexpr const char *TagStr<type>() { return #type; }    \
  template <> inline constexpr const char *TagStr<val>() { return #type; }     \
  template <> inline constexpr const char *TagStr<NBTTags::type>() {           \
    return #type;                                                              \
  }

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

// ============================================================================
//    Runtime tags conversion system
// ============================================================================

/**
 * @brief Get the tag for the given type's id
 *
 * @param T the nbt type enum
 * @return the corresponding string representation
 */
NBTTags Tag(uint8_t id);

/**
 * @brief Get the tag id for the given type's tag
 *
 * @param T the nbt type enum
 * @return the corresponding string representation
 */
uint8_t TagID(NBTTags t);

// ----------------------------------------------------------------------------

/**
 * @brief Get the tag str version for the given type's enum
 *
 * @param T the nbt type enum
 * @return the corresponding string representation
 */
const char *TagStr(NBTTags t);
/**
 * @brief Get the tag str version for the given type's id
 *
 * @param T the nbt type enum
 * @return the corresponding string representation
 */
inline const char *TagStr(uint8_t id) { return TagStr(Tag(id)); }

} // namespace minecraft::nbt

#endif