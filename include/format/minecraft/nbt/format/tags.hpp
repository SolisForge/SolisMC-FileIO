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
template <typename T> constexpr NBTTags::Tags Tag() {
  static_assert(!std::is_base_of<NBTType, T>::value,
                "The type should be NBT type.");
  if (solis::is_specialization<T, List>::value)
    return NBTTags::List;
  else if (std::is_base_of_v<Compound, T>)
    return NBTTags::Compound;
  return NBTTags::ERROR;
}

/**
 * @brief Get the tag str version for the given type's enum
 *
 * @tparam T the nbt type enum
 * @return the corresponding string representation
 */
template <NBTTags::Tags T> constexpr const char *TagStr() { return "ERROR"; }

/**
 * @brief Get the tag str version for the given type
 *
 * @tparam T the nbt type
 * @return the corresponding string representation
 */
template <typename T> inline constexpr const char *TagStr() {
  return TagStr<Tag<T>>();
}

// ============================================================================
//    Tags implementation
// ============================================================================

#define NBT_STR_FROM_VAL(type)                                                 \
  template <> inline constexpr const char *TagStr<NBTTags::type>() {           \
    return #type;                                                              \
  }
/*!
 * \def NBT_TAG_FROM_TYPE(type, val)
 * Register a NBT type - Type Enum - Type ID correspondance by making
 * specialization of conversion functions.
 */
#define NBT_TAG_FROM_TYPE(type)                                                \
  template <> inline constexpr NBTTags::Tags Tag<type>() {                     \
    return NBTTags::type;                                                      \
  }                                                                            \
  NBT_STR_FROM_VAL(type)

// ---------------------------------
// Define special tags
NBT_STR_FROM_VAL(END)
NBT_STR_FROM_VAL(ERROR)

// Define class tags
NBT_TAG_FROM_TYPE(Byte)
NBT_TAG_FROM_TYPE(Short)
NBT_TAG_FROM_TYPE(Int)
NBT_TAG_FROM_TYPE(Long)
NBT_TAG_FROM_TYPE(Float)
NBT_TAG_FROM_TYPE(Double)
NBT_TAG_FROM_TYPE(String)
NBT_TAG_FROM_TYPE(ByteArray)
NBT_TAG_FROM_TYPE(IntArray)
NBT_TAG_FROM_TYPE(LongArray)

// Define complex tags
NBT_STR_FROM_VAL(List)
NBT_TAG_FROM_TYPE(Compound)

#undef NBT_STR_FROM_VAL
#undef NBT_TAG_FROM_TYPE

// ============================================================================
//    Runtime tags conversion system
// ============================================================================

/**
 * @brief Get the tag str version for the given type's enum
 *
 * @param T the nbt type enum
 * @return the corresponding string representation
 */
inline const char *TagStr(TagID_t t) {
#define GENERATE_CASE(type)                                                    \
  case NBTTags::type:                                                          \
    return TagStr<NBTTags::type>();

  switch (t) {
    GENERATE_CASE(END)
    GENERATE_CASE(Byte)
    GENERATE_CASE(Short)
    GENERATE_CASE(Int)
    GENERATE_CASE(Long)
    GENERATE_CASE(Float)
    GENERATE_CASE(Double)
    GENERATE_CASE(ByteArray)
    GENERATE_CASE(String)
    GENERATE_CASE(List)
    GENERATE_CASE(Compound)
    GENERATE_CASE(IntArray)
    GENERATE_CASE(LongArray)
  default:
    return TagStr<NBTTags::ERROR>();
  }
#undef GENERATE_CASE
}

} // namespace minecraft::nbt

#endif