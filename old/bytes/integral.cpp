// ============================================================================
// Project: SOLISMC-FILEIO
//
// Integral-types NBT parser & writer implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      04/06/2026 (created 04/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/integral.hpp" // IWYU pragma: keep
#include "common.hpp"
#include "minecraft/io/nbt/bytes/interface.hpp"
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>

namespace minecraft::nbt::byte::base {

// ============================================================================
// Utils
// ============================================================================

/**
 * @brief Fill a integral value from bytes
 *
 * @param strm the stream to read from
 * @param n the number of bytes left in the stream
 * @param out the integral variable to fill
 * @return the number of bytes used
 */
template <std::integral T, std::endian origin>
constexpr uint8_t from_bytes(const Stream &strm, const Size &n, T &out,
                             uint8_t offset) {
  // Compute the loop properties
  constexpr auto TYPE_LENGTH = sizeof(T);
  const auto n_read = static_cast<uint8_t>(std::min(n, sizeof(T) - offset));

  for (uint8_t i = 0; i < n_read; i++) {
    // JAVA byte parsing (values are big-endian)
    if constexpr (origin == std::endian::big)
      out += cvt_byte<T>(strm[i])
             << ((TYPE_LENGTH - i - offset - 1) * BIT_PER_BYTE);
    // BEDROCK byte parsing (values are little-endian)
    else
      out += cvt_byte<T>(strm[i]) << ((i + offset) * BIT_PER_BYTE);
  }
  return n_read;
}

// ============================================================================

/**
 * @brief Fill a byte buffer from an integral value
 *
 * @param in the value to write
 * @param strm the buffer to write to
 * @param n the number of bytes left in the buffer
 * @return the number of bytes used
 */
template <std::integral T, std::endian dest>
constexpr uint8_t to_bytes(const T &in, Buffer &strm, const Size &n,
                           uint8_t offset) {
  // Compute the loop properties
  constexpr auto TYPE_LENGTH = sizeof(T);
  const auto n_write = static_cast<uint8_t>(std::min(n, sizeof(T) - offset));

  for (uint8_t i = 0; i < n_write; i++) {
    uint8_t shift;
    // JAVA byte writing (values are big-endian)
    if constexpr (dest == std::endian::big)
      shift = BIT_PER_BYTE * (TYPE_LENGTH - 1 - i - offset);
    // BEDROCK byte writing (values are little-endian)
    else
      shift = BIT_PER_BYTE * (i + offset);
    strm[i] = static_cast<int8_t>(
        static_cast<uint8_t>((in & (((T)0xff) << shift)) >> shift));
  }
  return n_write;
}

// ============================================================================
// Byte parser
// ============================================================================

template <std::integral T, GameVersion GV>
ParseResult IntegralByteParser<T, GV>::parse(Stream &strm, Size &n) {
  // If we are reading a new value
  if (read_bytes == TYPE_LENGTH)
    reset();

  // Read more bytes into the integral value
  auto n_bytes =
      from_bytes<T, NBTEndianness<GV>::value>(strm, n, value, read_bytes);
  read_bytes += n_bytes;
  strm += n_bytes;
  n -= n_bytes;
  return (read_bytes == TYPE_LENGTH) ? ParseResult::ENDED
                                     : ParseResult::UNFINISHED;
}

template <std::integral T, GameVersion GV>
void IntegralByteParser<T, GV>::reset() {
  read_bytes = 0;
  value = 0;
}

// ============================================================================
// Byte writer
// ============================================================================

template <std::integral T, GameVersion GV>
WriteResult IntegralByteWriter<T, GV>::write(Buffer &strm, Size &n) {
  // Reset parser if done
  if (is_done())
    reset();

  // Write more bytes into the buffer
  auto n_bytes =
      to_bytes<T, NBTEndianness<GV>::value>(value, strm, n, written_bytes);
  written_bytes += n_bytes;
  strm += n_bytes;
  n -= n_bytes;
  return (is_done()) ? WriteResult::ENDED : WriteResult::UNFINISHED;
}

// ============================================================================
template <std::integral T, GameVersion GV>
void IntegralByteWriter<T, GV>::reset() {
  value = 0;
  written_bytes = 0;
}

// ============================================================================
// Force definition of these ByteParser in this library
// ============================================================================
EXPORT_COMMON_NBT_PARSER(GameVersion::JAVA, int8_t, int16_t, int32_t, int64_t);
EXPORT_COMMON_NBT_PARSER(GameVersion::JAVA, uint8_t, uint16_t, uint32_t,
                         uint64_t);
EXPORT_COMMON_NBT_PARSER(GameVersion::BEDROCK, int8_t, int16_t, int32_t,
                         int64_t);
EXPORT_COMMON_NBT_PARSER(GameVersion::BEDROCK, uint8_t, uint16_t, uint32_t,
                         uint64_t);
EXPORT_COMMON_NBT_WRITER(GameVersion::JAVA, int8_t, int16_t, int32_t, int64_t);
EXPORT_COMMON_NBT_WRITER(GameVersion::JAVA, uint8_t, uint16_t, uint32_t,
                         uint64_t);
EXPORT_COMMON_NBT_WRITER(GameVersion::BEDROCK, int8_t, int16_t, int32_t,
                         int64_t);
EXPORT_COMMON_NBT_WRITER(GameVersion::BEDROCK, uint8_t, uint16_t, uint32_t,
                         uint64_t);

} // namespace minecraft::nbt::byte::base
