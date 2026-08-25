// ============================================================================
// Project: SOLISMC-IO
//
// Implementation of base functions for integral type byte parsing
//
// Author    Meltwin (github@meltwin.fr)
// Date      10/08/2026 (created 10/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/base/integral.hpp"
#include "minecraft/io/nbt/bytes/base/common.hpp"
#include <cmath>
#include <cstdint>

namespace minecraft::nbt::byte::base {

// ============================================================================
// Generic implementation
// ============================================================================

/**
 * @brief Partial int parsing from bytes
 *
 * @tparam data_endianness endianness of the encoded data
 * @param strm stream to read from
 * @param state the current state of the parsing
 * @param tlen the number of bytes for this type
 * @param value the value to fill
 * @return ParseResult the result of the parsing
 */
template <std::endian data_endianness>
inline ParseResult read_partial_int(Stream &strm, IntParseState &state,
                                    const uint8_t tlen, char *value) {
  auto to_read = std::min(strm.n, state.left(tlen));

  auto end_index = state.read_int + to_read;
  for (uint8_t i = state.read_int; i < end_index; i++) {
    // Data endianness is the same as system
    if constexpr (std::endian::native == data_endianness)
      value[i] = strm.data[0];
    // Data is stored in reverse order
    else
      value[tlen - i - 1] = strm.data[0];

    strm.inc();
    state.read_int += 1;
  }
  return (state.read_int == tlen) ? ParseResult::ENDED
                                  : ParseResult::UNFINISHED;
}

// ============================================================================
// Bindings
// ============================================================================
#define BIND(endianess)                                                        \
  template <>                                                                  \
  ParseResult read_integral<endianess>(Stream & strm, IntParseState & state,   \
                                       const uint8_t tlen, char *value) {      \
    return read_partial_int<endianess>(strm, state, tlen, value);              \
  }

BIND(std::endian::little)
BIND(std::endian::big)

} // namespace minecraft::nbt::byte::base

// ============================================================================
// Export
// ============================================================================
namespace minecraft::nbt::byte {

#define ARGS(T) Stream &strm, IntParseState &state, T &value
#define EXPORT(type)                                                           \
  template ParseResult read_int<type, GameVersion::JAVA>(ARGS(type));          \
  template ParseResult read_int<type, GameVersion::BEDROCK>(ARGS(type));

EXPORT(int8_t);
EXPORT(int16_t);
EXPORT(int32_t);
EXPORT(int64_t);

} // namespace minecraft::nbt::byte