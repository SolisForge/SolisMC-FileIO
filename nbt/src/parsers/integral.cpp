// ============================================================================
// Project: SOLISMC-FILEIO
//
// Integral types byte-parsing implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/12/2025 (created 26/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parsers/integral.hpp"

namespace minecraft::nbt {

template <std::integral T>
ParseResult BytesParser<T>::parse(const StreamChar *&strm, unsigned long &N) {
  // Reset parser before parsing a new value (to prevent the calling of
  // reset() by other programs)
  if (is_parsed())
    reset();

  NBT_PARSE_N_BYTE_BEGIN()
#if NBT_BIG_ENDIAN == 0
  // BEDROCK byte parsing (values are little-endian)
  value_ += static_cast<T>(strm[0]) << (n_bytes * BIT_PER_BYTES);
#else
  // JAVA byte parsing (values are big-endian)
  value_ += static_cast<T>(strm[0])
            << ((TYPE_LENGTH - n_bytes - 1) * BIT_PER_BYTES);
#endif
  NBT_PARSE_N_BYTE_END(n_bytes, TYPE_LENGTH);

  // Prepare parser reset for next iteration
  n_bytes = 0;
  return ParseResult::SUCCESS;
}

// Force definition of these ByteParser in this library
template struct BytesParser<int8_t>;
template struct BytesParser<uint8_t>;
template struct BytesParser<int16_t>;
template struct BytesParser<uint16_t>;
template struct BytesParser<int32_t>;
template struct BytesParser<uint32_t>;
template struct BytesParser<int64_t>;
template struct BytesParser<uint64_t>;
} // namespace minecraft::nbt
