// ============================================================================
// Project: SOLISMC-IO
//
// Definition of base functions for integral type byte parsing
//
// Author    Meltwin (github@meltwin.fr)
// Date      10/08/2026 (created 10/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once

#include "minecraft/game_info.hxx"
#include "minecraft/io/nbt/bytes/base/common.hxx"
#include <bit>
#include <concepts>
#include <cstdint>

namespace minecraft::nbt::byte {

// ============================================================================
// Base implementation
// ============================================================================
struct IntParseState {
  uint8_t read_char{0};

  inline auto left(uint8_t tlen) const {
    return static_cast<std::size_t>(tlen - read_char);
  }
};

// ============================================================================

namespace base {
#define ARGS_IMPL                                                              \
  Stream &strm, IntParseState &state, const uint8_t tlen, char *value

/**
 * @brief Parse an integral type
 *
 * @param strm stream to read from
 * @param state parsing state
 * @param tlen length of the integral type to parse
 * @param value pointer to the value to set
 * @return result of the parsing
 */
template <std::endian data_endianness> ParseResult read_integral(ARGS_IMPL);

// Specializations
template <> ParseResult read_integral<std::endian::little>(ARGS_IMPL);
template <> ParseResult read_integral<std::endian::big>(ARGS_IMPL);

#undef ARGS_IMPL
} // namespace base

// ============================================================================
// Bindings
// ============================================================================
#define ARGS(T) Stream &strm, IntParseState &state, T &value

// Java implementation
// ----------------------------------------------------------------------------
namespace java {

/**
 * @brief Parse an integral value from the stream
 *
 * @tparam T integral type to parse
 * @param strm stream to read from
 * @param state parsing state
 * @param value value object to fill
 * @return result of the parsing
 */
template <std::signed_integral T> inline ParseResult read_int(ARGS(T)) {
  return base::read_integral<std::endian::big>(strm, state, sizeof(T),
                                               (char *)(&value));
}

} // namespace java

// Bedrock implementation
// ----------------------------------------------------------------------------
namespace bedrock {

/**
 * @brief Parse an integral value from the stream
 *
 * @tparam T integral type to parse
 * @param strm stream to read from
 * @param state parsing state
 * @param value value object to fill
 * @return result of the parsing
 */
template <std::signed_integral T> inline ParseResult read_int(ARGS(T)) {
  return base::read_integral<std::endian::little>(strm, state, sizeof(T),
                                                  (char *)(&value));
}

} // namespace bedrock

// General-purpose implementation
// ----------------------------------------------------------------------------
#define ARGS_FWD strm, state, value

/**
 * @brief Parse an integral value from the stream
 *
 * @tparam T integral type to parse
 * @tparam GV targeted game version
 * @param strm stream to read from
 * @param state parsing state
 * @param value value object to fill
 * @return result of the parsing
 */
template <std::signed_integral T, GameVersion GV = GameVersion::JAVA>
ParseResult read_int(ARGS(T)) {
  if constexpr (GV == GameVersion::JAVA)
    return java::read_int(ARGS_FWD);
  return bedrock::read_int(ARGS_FWD);
}

#undef ARGS_FWD

// ============================================================================
// Exports
// ============================================================================
#define EXPORT(type)                                                           \
  extern template ParseResult read_int<type, GameVersion::JAVA>(ARGS(type));   \
  extern template ParseResult read_int<type, GameVersion::BEDROCK>(ARGS(type));

EXPORT(int8_t)
EXPORT(int16_t)
EXPORT(int32_t)
EXPORT(int64_t)

#undef EXPORT
#undef ARGS

} // namespace minecraft::nbt::byte