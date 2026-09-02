// ============================================================================
// Project: SOLISMC_IO
//
// Byte manipulation helpers for NBT parsing & reading
//
// Author    Meltwin (github@meltwin.fr)
// Date      02/09/2026 (created 02/09/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once

#include "minecraft/io/nbt/bytes/base/common.hxx"

#include <algorithm>
#include <bit>

using minecraft::nbt::byte::Stream;

namespace helper {

// ============================================================================
/**
 * @brief Copy n bytes from one buffer to another
 *
 * @tparam data_endianness in which endianness is the data stored in
 * @param strm stream to read bytes from
 * @param dest buffer to write bytes into
 * @param type_length length of the type we are copying
 * @param offset offset to allow partial type reading (reading by block)
 * @return number of copied bytes
 */
template <std::endian data_endianness>
uint8_t copy_bytes(Stream &strm, char *dest, uint8_t type_length,
                   uint8_t offset) {
  // Get copy positions
  const auto copy_length = std::min((std::size_t)type_length - offset, strm.n);
  auto src_start = strm.data;
  auto src_end = strm.data + copy_length;
  auto dest_start = dest + offset;

  // Copy the bytes in the same order as the system
  if constexpr (std::endian::native == data_endianness)
    std::copy(src_start, src_end, dest_start);
  else
    std::reverse_copy(src_start, src_end, dest_start);
  strm.inc(copy_length);
  return (uint8_t)copy_length;
}

} // namespace helper