// ============================================================================
// Project: SOLISMC-FILEIO
//
// Common utility functions for nbt parsing & writing
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/06/2026 (created 12/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_BYTE_COMMON
#define SOLISMC_IO_NBT_BYTE_COMMON

#include "minecraft/io/nbt/bytes/interface.hpp"
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace minecraft::nbt {

/**
 * @brief Helper function for static cast between a char to a T
 *
 * @tparam T the type in which to static cast
 * @param c the byte to cast
 */
template <typename T> constexpr T cvt_byte(char c) {
  return static_cast<T>(static_cast<uint8_t>(c));
}

// ============================================================================

/**
 * @brief Copy N bytes from the given buffer to the other one, according to
 *
 * @tparam N The maximum number of bytes to write in the out buffer (= size of
 * the out buffer)
 * @param in The input buffer where to read the bytes from
 * @param in_length How many bytes are available in the input buffer
 * @param out The output buffer where to copy the bytes
 * @param out_length How many bytes are available in the output buffer
 * @param offset How many bytes were already read into the input buffer
 * @return the number of copied bytes
 */
template <std::endian origin, std::endian destination>
constexpr uint8_t copy_bytes(const Stream *in, const std::size_t &in_length,
                             Stream *out, const std::size_t &out_length,
                             uint8_t offset) {
  if constexpr (origin == destination) {
    // Directly copy the bytes
    const auto n_copied = std::min(out_length - offset, in_length);
    memcpy(out, in, n_copied);
  } else {
    // Reverse bytes
  }
  return 0;
}

/**
 * @brief Shorthand for copy_bytes when the destination is a variable.
 *
 * @tparam N The maximum number of bytes to write in the out buffer (= size of
 * the out buffer)
 * @param in The input buffer where to read the bytes from
 * @param in_length How many bytes are available in the input buffer
 * @param out The output buffer where to copy the bytes
 * @param out_length How many bytes are available in the output buffer
 * @param offset How many bytes were already read into the input buffer
 * @return the number of copied bytes
 */
template <std::endian origin>
constexpr uint8_t
import_from_bytes(const Stream *in, const std::size_t &in_length, Stream *out,
                  const std::size_t &out_length, uint8_t offset) {
  return copy_bytes<origin, std::endian::native>(in, in_length, out, out_length,
                                                 offset);
}
/**
 * @brief Shorthand for copy_bytes when the destination is a byte stream.
 *
 * @tparam N The maximum number of bytes to write in the out buffer (= size of
 * the out buffer)
 * @param in The input buffer where to read the bytes from
 * @param in_length How many bytes are available in the input buffer
 * @param out The output buffer where to copy the bytes
 * @param out_length How many bytes are available in the output buffer
 * @param offset How many bytes were already read into the input buffer
 * @return the number of copied bytes
 */
template <std::endian destination>
constexpr uint8_t
export_to_bytes(const Stream *in, const std::size_t &in_length, Stream *out,
                const std::size_t &out_length, uint8_t offset) {
  return copy_bytes<std::endian::native, destination>(in, in_length, out,
                                                      out_length, offset);
}

} // namespace minecraft::nbt

#endif