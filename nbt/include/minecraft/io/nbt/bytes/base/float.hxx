// ============================================================================
// Project: SOLISMC_IO
//
// Definition of base functions for floating-point type byte parsing
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once

#include "minecraft/game_info.hxx"
#include "minecraft/io/nbt/bytes/base/common.hxx"
#include <bit>
#include <concepts>

namespace minecraft::nbt::byte {

// ============================================================================
// Base implementation
// ============================================================================
struct FloatParseState {
  uint8_t read_char{0};

  inline auto left(uint8_t tlen) const {
    return static_cast<std::size_t>(tlen - read_char);
  }
};

// ============================================================================

namespace base {
#define ARGS_IMPL                                                              \
  Stream &strm, FloatParseState &state, const uint8_t tlen, char *value

/**
 * @brief Parse a floating-point type from bytes
 *
 * @param strm stream to read from
 * @param state parsing state
 * @param tlen length of the integral type to parse
 * @param value pointer to the value to set
 * @return result of the parsing
 */
template <std::endian data_endianness> ParseResult read_floating(ARGS_IMPL);

// Specializations
template <> ParseResult read_floating<std::endian::little>(ARGS_IMPL);
template <> ParseResult read_floating<std::endian::big>(ARGS_IMPL);

#undef ARGS_IMPL
} // namespace base

// ============================================================================
// Bindings
// ============================================================================
#define ARGS(T) Stream &strm, FloatParseState &state, T &value

// Java implementation
// ----------------------------------------------------------------------------
namespace java {

/**
 * @brief Parse a floating-point value from the stream
 *
 * @tparam T floating-point type to parse
 * @param strm stream to read from
 * @param state parsing state
 * @param value value object to fill
 * @return result of the parsing
 */
template <std::floating_point T> inline ParseResult read_float(ARGS(T)) {
  return base::read_floating<std::endian::big>(strm, state, sizeof(T),
                                               (char *)(&value));
}

} // namespace java

// Bedrock implementation
// ----------------------------------------------------------------------------
namespace bedrock {

/**
 * @brief Parse a floating-point value from the stream
 *
 * @tparam T floating-point type to parse
 * @param strm stream to read from
 * @param state parsing state
 * @param value value object to fill
 * @return result of the parsing
 */
template <std::floating_point T> inline ParseResult read_float(ARGS(T)) {
  return base::read_floating<std::endian::little>(strm, state, sizeof(T),
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
template <std::floating_point T, GameVersion GV = GameVersion::JAVA>
ParseResult read_float(ARGS(T)) {
  if constexpr (GV == GameVersion::JAVA)
    return java::read_float(ARGS_FWD);
  return bedrock::read_float(ARGS_FWD);
}

#undef ARGS_FWD

// ============================================================================
// Exports
// ============================================================================
#define EXPORT(type)                                                           \
  extern template ParseResult read_float<type, GameVersion::JAVA>(ARGS(type)); \
  extern template ParseResult read_float<type, GameVersion::BEDROCK>(          \
      ARGS(type));

EXPORT(float)
EXPORT(double)

#undef EXPORT
#undef ARGS

} // namespace minecraft::nbt::byte