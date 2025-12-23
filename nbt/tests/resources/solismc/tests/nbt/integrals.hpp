// ============================================================================
// Project: SOLISMC_FILEIO
//
// This file contains test values for NBT integral parsing
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/11/2025 (created 26/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#ifndef SOLIS_MC_NBT_INTEGRALS_TEST_SET_HPP
#define SOLIS_MC_NBT_INTEGRALS_TEST_SET_HPP

#include <cstdint>
#include <solismc/tests/nbt/common.hpp>

/**
 * @brief Structure to represent the bytes representation of an integral value.
 *
 * @tparam T the output type
 * @tparam Bytes... the bytes representation (big-endian)
 */
template <typename T, solis::StreamChar... Bytes>
struct IntegralValue : solis::NBTValue<T, Bytes...> {

  explicit constexpr IntegralValue<T, Bytes...>(const T &value)
      : solis::NBTValue<T, Bytes...>(value) {}
};

// ==========================================================================
// Bytes
// ==========================================================================
#define MK_BYTE(name, b1, value)                                               \
  [[maybe_unused]] constexpr auto name = IntegralValue<int8_t, b1>(value);

MK_BYTE(BYTE_1, '\x19', 25);
MK_BYTE(BYTE_2, '\x27', 39);
MK_BYTE(BYTE_3, '\x53', 83);
MK_BYTE(BYTE_4, (solis::StreamChar)'\x89', -119);

#undef MK_BYTE

// ==========================================================================
// Shorts
// ==========================================================================
// Define short values
#define MK_SHORT(name, b1, b2, value)                                          \
  [[maybe_unused]] constexpr auto name =                                       \
      solis::Combined<int16_t>(b1, b2, value);

MK_SHORT(SHORT_1, BYTE_1, BYTE_2, 6439);   // x19 27 (big endian)
MK_SHORT(SHORT_2, BYTE_3, BYTE_4, 21385);  // x53 89 (big endian)
MK_SHORT(SHORT_3, BYTE_3, BYTE_1, 21273);  // x53 19 (big endian)
MK_SHORT(SHORT_4, BYTE_4, BYTE_1, -30439); // x89 19 (big endian)

#undef MK_SHORT

// Define short streams to parse
constexpr auto ONE_SHORT = solis::NBTstream(SHORT_1);
constexpr auto TWO_SHORT = ONE_SHORT + SHORT_2;
constexpr auto INCOMPLETE_THREE_SHORTS = TWO_SHORT + BYTE_3;
constexpr auto NEGATIVE_SHORT = solis::NBTstream(SHORT_4);

// ==========================================================================
// Int
// ==========================================================================
#define MK_INT(name, short1, short2, value)                                    \
  [[maybe_unused]] constexpr auto name =                                       \
      solis::Combined<int32_t>(short1, short2, value);

MK_INT(INT_1, SHORT_1, SHORT_2, 422007689)   // x19 27 53 89 (big endian)
MK_INT(INT_2, SHORT_1, SHORT_3, 422007577)   // x19 27 53 19 (big endian)
MK_INT(INT_3, SHORT_4, SHORT_3, -1994829031) // x89 19 53 19 (big endian)

#undef MK_INT

// Define int streams to parse
constexpr auto ONE_INT = solis::NBTstream(INT_2);
constexpr auto TWO_INT = ONE_INT + INT_3;
constexpr auto INCOMPLETE_THREE_INTS = TWO_INT + BYTE_3;
constexpr auto NEGATIVE_INT = solis::NBTstream(INT_3);

// ==========================================================================
// Long
// ==========================================================================
#define MK_LONG(name, int1, int2, value)                                       \
  [[maybe_unused]] constexpr auto LONG_##name =                                \
      solis::Combined<int64_t>(int1, int2, value);

// x19 27 53 89 19 27 53 19 (big endian)
MK_LONG(1, INT_1, INT_2, 1812509223337546521)
// x19 27 53 19 19 27 53 89 (big endian)
MK_LONG(2, INT_2, INT_1, 1812508742301209481)
// x89 19 53 19 19 27 53 19 (big endian)
MK_LONG(3, INT_3, INT_2, -8567725448834362599)

#undef MK_LONG

// Define int streams to parse
constexpr auto ONE_LONG = solis::NBTstream(LONG_1);
constexpr auto TWO_LONG = ONE_LONG + LONG_2;
constexpr auto INCOMPLETE_THREE_LONGS = TWO_LONG + BYTE_3;
constexpr auto NEGATIVE_LONG = solis::NBTstream(LONG_3);

#endif