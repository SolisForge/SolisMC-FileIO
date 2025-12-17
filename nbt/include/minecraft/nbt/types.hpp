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
#include <exception>
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

[[maybe_unused]] static constexpr const char *getName(Tags tag) {
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
 * @brief Exception to be run when using NBT-related functions with
 * C++ types that have not been registered.
 */
struct NBTUnregisteredType : std::exception {
  const char *what() const noexcept final {
    return "No matching specialization found for this type";
  }
};

// ============================================================================

/**
 * @brief STD type to NBT tag mapping
 *
 * @tparam T
 * @return constexpr Tags::E
 */
template <typename T> constexpr Tags getTag() {
  throw NBTUnregisteredType();
  return Tags::ERROR;
}

// ============================================================================

/**
 * @brief Meta-class for storing the NBT structure type info
 * @tparam T      the STD type representation for this tag
 * @tparam NBTTag the associated NBT tag
 */
template <typename T, Tags NBTTag = getTag<T>()> struct NBTTypeInfo {
  using type = T;
  constexpr Tags tag() const { return NBTTag; }
};

// ============================================================================
// Type registration
// ============================================================================
// Integral types
template <> constexpr Tags getTag<int8_t>() { return Tags::Byte; }
template <> constexpr Tags getTag<uint8_t>() { return Tags::Byte; }
using Byte = NBTTypeInfo<int8_t>;
template <> constexpr Tags getTag<int16_t>() { return Tags::Short; }
template <> constexpr Tags getTag<uint16_t>() { return Tags::Short; }
using Short = NBTTypeInfo<int16_t>;
template <> constexpr Tags getTag<int32_t>() { return Tags::Int; }
template <> constexpr Tags getTag<uint32_t>() { return Tags::Int; }
using Int = NBTTypeInfo<int32_t>;
template <> constexpr Tags getTag<int64_t>() { return Tags::Long; }
template <> constexpr Tags getTag<uint64_t>() { return Tags::Long; }
using Long = NBTTypeInfo<int64_t>;

// Floating point
template <> constexpr Tags getTag<float>() { return Tags::Float; }
using Float = NBTTypeInfo<float>;
template <> constexpr Tags getTag<double>() { return Tags::Double; }
using Double = NBTTypeInfo<double>;

} // namespace minecraft::nbt

#endif