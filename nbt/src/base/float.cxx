// ============================================================================
// Project: SOLISMC_IO
//
// Implementation of base functions for floating-point type byte parsing
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/base/float.hxx"

#include "byte_helper.hxx"

namespace minecraft::nbt::byte::base {

// ============================================================================
// Implementation for each endianness
// ============================================================================
#define BIND(endianess)                                                        \
  template <>                                                                  \
  ParseResult read_floating<endianess>(Stream & strm, FloatParseState & state, \
                                       const uint8_t tlen, char *value) {      \
    auto read =                                                                \
        helper::copy_bytes<endianess>(strm, value, tlen, state.read_char);     \
    state.read_char += read;                                                   \
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

#define ARGS(T) Stream &strm, FloatParseState &state, T &value
#define EXPORT(type)                                                           \
  template ParseResult read_float<type, GameVersion::JAVA>(ARGS(type));        \
  template ParseResult read_float<type, GameVersion::BEDROCK>(ARGS(type));

EXPORT(float);
EXPORT(double);

} // namespace minecraft::nbt::byte