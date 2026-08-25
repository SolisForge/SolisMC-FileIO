// ============================================================================
// Project: SOLISMC-FILEIO
//
// String-types NBT parser & writer implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/06/2026 (created 13/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/string.hpp"
#include "minecraft/io/nbt/bytes/interface.hpp"
#include <cstring>

namespace minecraft::nbt::byte::base {

// ============================================================================

template <GameVersion GV>
ParseResult StringByteParser<GV>::parse(Stream &strm, Size &n) {
  // If last parsing was done, reset values
  if (is_done())
    reset();

  // Parse the size of the string
  if (!int_parser.is_done()) {
    if (auto ret = int_parser.parse(strm, n); ret != ParseResult::ENDED)
      return ret;

    // Initialize string of given length
    value = std::string(int_parser.get(), '\x00');
  }

  // Copy bytes
  auto p = &value.data()[read_bytes];
  auto n_bytes = std::min((std::size_t)int_parser.get() - read_bytes, n);
  std::memcpy(p, strm, n_bytes);

  // Register copied bytes
  read_bytes += n_bytes;
  strm += n_bytes;
  n -= n_bytes;

  // Return parsing state
  return (read_bytes == int_parser.get()) ? ParseResult::ENDED
                                          : ParseResult::UNFINISHED;
}

// ============================================================================

template <GameVersion GV> bool StringByteParser<GV>::is_done() const {
  return (read_bytes > 0) && int_parser.is_done() && read_bytes == value.size();
}

// ============================================================================

template <GameVersion GV> void StringByteParser<GV>::reset() {
  value = std::string{};
  read_bytes = 0;
  int_parser.reset();
}

// ============================================================================
// Force definition of these ByteParser in this library
// ============================================================================
EXPORT_TEMPLATE(StringByteParser<GameVersion::JAVA>);
EXPORT_TEMPLATE(StringByteParser<GameVersion::BEDROCK>);
EXPORT_COMMON_NBT_PARSER(GameVersion::JAVA, std::string);
EXPORT_COMMON_NBT_PARSER(GameVersion::BEDROCK, std::string);

} // namespace minecraft::nbt::byte::base