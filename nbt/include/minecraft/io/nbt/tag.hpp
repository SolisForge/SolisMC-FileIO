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
#include <string>
#include <vector>

namespace minecraft::nbt {

// ============================================================================
// Tag definition
// ============================================================================
enum class Tag : uint8_t {
#define X(name, value) name = value,
#include ".xmacros/x_nbt.hpp"
#undef X
};

// ============================================================================
// Tag <-> types
// ============================================================================
template <Tag T> struct TagType {};
#define MK_TAG_TYPE_STRUCT(tag, type)                                          \
  template <> struct TagType<Tag::tag> { using C = type; };
MK_TAG_TYPE_STRUCT(BYTE, int8_t)
MK_TAG_TYPE_STRUCT(SHORT, int16_t);
MK_TAG_TYPE_STRUCT(INT, int32_t);
MK_TAG_TYPE_STRUCT(LONG, int64_t);
MK_TAG_TYPE_STRUCT(FLOAT, float);
MK_TAG_TYPE_STRUCT(DOUBLE, double);
MK_TAG_TYPE_STRUCT(BYTE_ARRAY, std::vector<int8_t>);
MK_TAG_TYPE_STRUCT(INT_ARRAY, std::vector<int32_t>);
MK_TAG_TYPE_STRUCT(LONG_ARRAY, std::vector<int64_t>);
MK_TAG_TYPE_STRUCT(STRING, std::string);
#undef MK_TAG_TYPE_STRUCT

template <typename T> struct TypeToTag {
  static constexpr Tag TAG{Tag::COMPOUND};
};
#define MK_TAG_TYPE_STRUCT(tag, type)                                          \
  template <> struct TypeToTag<type> { static constexpr Tag TAG{Tag::tag}; };
MK_TAG_TYPE_STRUCT(BYTE, int8_t);
MK_TAG_TYPE_STRUCT(BYTE, uint8_t);
MK_TAG_TYPE_STRUCT(SHORT, int16_t);
MK_TAG_TYPE_STRUCT(SHORT, uint16_t);
MK_TAG_TYPE_STRUCT(INT, int32_t);
MK_TAG_TYPE_STRUCT(INT, uint32_t);
MK_TAG_TYPE_STRUCT(LONG, int64_t);
MK_TAG_TYPE_STRUCT(LONG, uint64_t);

MK_TAG_TYPE_STRUCT(FLOAT, float);
MK_TAG_TYPE_STRUCT(DOUBLE, double);

template <typename T> struct TypeToTag<std::vector<T>> {
  static constexpr Tag TAG{Tag::LIST};
};
MK_TAG_TYPE_STRUCT(BYTE_ARRAY, std::vector<int8_t>);
MK_TAG_TYPE_STRUCT(BYTE_ARRAY, std::vector<uint8_t>);
MK_TAG_TYPE_STRUCT(INT_ARRAY, std::vector<int32_t>);
MK_TAG_TYPE_STRUCT(INT_ARRAY, std::vector<uint32_t>);
MK_TAG_TYPE_STRUCT(LONG_ARRAY, std::vector<int64_t>);
MK_TAG_TYPE_STRUCT(LONG_ARRAY, std::vector<uint64_t>);

MK_TAG_TYPE_STRUCT(STRING, std::string);
#undef MK_TAG_TYPE_STRUCT

// ============================================================================
// Tag <-> int, str, ...
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

constexpr const char *to_str(const Tag tag) {
// Define X macro
#define X(name, ...)                                                           \
  case Tag::name:                                                              \
    return #name;

  switch (tag) {
#include ".xmacros/x_nbt.hpp"
  default:
    return "UNKNOWN";
  }
#undef X
}

} // namespace minecraft::nbt

#endif