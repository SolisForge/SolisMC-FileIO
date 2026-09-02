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
#include "minecraft/io/nbt/bytes/base/integral.hxx"
#include <cstdint>
#include <iostream>

#include "byte_helper.hxx"

namespace minecraft::nbt::byte::base {

// ============================================================================
// Implementation for each endianness
// ============================================================================
#define BIND(endianess)                                                        \
  template <>                                                                  \
  ParseResult read_integral<endianess>(Stream & strm, IntParseState & state,   \
                                       const uint8_t tlen, char *value) {      \
    auto read =                                                                \
        helper::copy_bytes<endianess>(strm, value, tlen, state.read_char);     \
    state.read_char += read;                                                   \
    std::cout << "Current state = " << (int)state.read_char                    \
              << " / type length = " << (int)tlen << std::endl;                \
    return (state.read_char == tlen) ? ParseResult::ENDED                      \
                                     : ParseResult::UNFINISHED;                \
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