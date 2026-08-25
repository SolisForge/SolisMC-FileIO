// ============================================================================
// Project: SOLISMC-FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/06/2026 (created 13/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/float.hpp"
#include "minecraft/io/nbt/bytes/interface.hpp"
#include <bit>

namespace minecraft::nbt::byte::base {

template <std::floating_point T, GameVersion GV>
ParseResult FloatByteParser<T, GV>::parse(Stream &strm, Size &n) {
  // Parse the floating point as an integral type
  if (auto ret = int_parser.parse(strm, n); ret != ParseResult::ENDED)
    return ret;

  // Cast the buffer as a floating point value
  value = std::bit_cast<T>(int_parser.get());
  return ParseResult::ENDED;
}

template <std::floating_point T, GameVersion GV>
void FloatByteParser<T, GV>::reset() {
  value = 0.0;
}

// ============================================================================
// Force definition of these ByteParser in this library
// ============================================================================
EXPORT_COMMON_NBT_PARSER(GameVersion::JAVA, float, double)
EXPORT_COMMON_NBT_PARSER(GameVersion::BEDROCK, float, double);

} // namespace minecraft::nbt::byte::base