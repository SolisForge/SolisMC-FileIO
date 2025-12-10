// ============================================================================
// Project: SOLISMC_FILEIO
//
// Definition of the common base of NBT structure.
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/11/2025 (created 13/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_TYPES_BASE_HPP
#define SOLISMC_NBT_TYPES_BASE_HPP

#include <cstdint>
#include <stdexcept>

namespace minecraft::nbt {

using TagID_t = uint8_t;

/**
 * @brief NBT tags byte values
 */
enum class Tags : TagID_t {
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
  ERROR = 255,

  // Special tag for defined NBT objects (e.g. level metadata, ...)
  Object = Compound,
};

static constexpr const char *getName(Tags tag) {
  // Macro to easily generate all the conversions
#define MK_CASE(type)                                                          \
  case Tags::type:                                                             \
    return #type

  // Construct
  switch (tag) {
    MK_CASE(END);
    MK_CASE(Byte);
    MK_CASE(Short);
    MK_CASE(Int);
    MK_CASE(Long);
    MK_CASE(Float);
    MK_CASE(Double);
    MK_CASE(String);
    MK_CASE(ByteArray);
    MK_CASE(List);
    MK_CASE(IntArray);
    MK_CASE(LongArray);
    MK_CASE(Compound);
    MK_CASE(ERROR);
  }

#undef MK_CASE
}

// ============================================================================

/**
 * @brief Meta-class for storing the NBT structure type info
 * @tparam T      the STD type representation for this tag
 * @tparam NBTTag the associated NBT tag
 */
template <typename T, Tags NBTTag> struct NBTTypeInfo {
  using type = T;
  constexpr Tags tag() const { return NBTTag; }
};

// ============================================================================

/**
 * @brief STD type to NBT tag mapping
 *
 * @tparam T
 * @return constexpr Tags::E
 */
template <typename T> constexpr Tags getTag() {
  throw std::runtime_error("Trying to get tag of unregistered type");
  return Tags::ERROR;
}

} // namespace minecraft::nbt

#endif