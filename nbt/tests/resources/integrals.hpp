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

#include <common.hpp>
#include <cstdint>

/**
 * @brief Structure to represent a stream based on a combination of several
 *
 * @tparam T the output typename
 * @tparam Bytes
 */
template <typename T, uint8_t... Bytes>
struct IntegralValue : solis::NBTValue<T, Bytes...> {

  constexpr IntegralValue<T, Bytes...>()
      : solis::NBTValue<T, Bytes...>(_get_value(0, Bytes...)) {}

  constexpr IntegralValue<T, Bytes...>(const T &value)
      : solis::NBTValue<T, Bytes...>(value) {}

private:
  static constexpr T _get_value(std::size_t i, uint8_t val) {
    return val << (8 * i);
  }

  template <typename... Args>
  static constexpr T _get_value(std::size_t i, uint8_t val, Args... args) {
    return _get_value(i, val) + _get_value(i + 1, args...);
  }
};

// ==========================================================================
// Bytes
// ==========================================================================
#define MK_BYTE(name, b1)                                                      \
  [[maybe_unused]] constexpr auto name = IntegralValue<int8_t, b1>();

MK_BYTE(BYTE_1, (uint8_t)'\x19');
MK_BYTE(BYTE_2, (uint8_t)'\x27');
MK_BYTE(BYTE_3, (uint8_t)'\x53');
MK_BYTE(BYTE_4, (uint8_t)'\x89');

#undef MK_BYTE

// ==========================================================================
// Shorts
// ==========================================================================
// Define short values
#define MK_SHORT(name, b1, b2)                                                 \
  [[maybe_unused]] constexpr auto name = solis::Combined<int16_t>(b1, b2);

MK_SHORT(SHORT_1, BYTE_1, BYTE_2);
MK_SHORT(SHORT_2, BYTE_3, BYTE_4);
MK_SHORT(SHORT_3, BYTE_3, BYTE_1);
MK_SHORT(SHORT_4, BYTE_4, BYTE_1);

#undef MK_SHORT

// Define short streams to parse
constexpr auto ONE_SHORT = solis::NBTstream(SHORT_1);
constexpr auto TWO_SHORT = solis::NBTstream(SHORT_1) + SHORT_2;
constexpr auto INCOMPLETE_THREE_SHORTS = TWO_SHORT + BYTE_3;
constexpr auto NEGATIVE_SHORT = solis::NBTstream(SHORT_2);

// ==========================================================================
// Int
// ==========================================================================
#define MK_INT(name, short1, short2)                                           \
  [[maybe_unused]] constexpr auto name =                                       \
      solis::Combined<int32_t>(short1, short2);

MK_INT(INT_12, SHORT_1, SHORT_2)
MK_INT(INT_13, SHORT_1, SHORT_3)
MK_INT(INT_14, SHORT_1, SHORT_4)
MK_INT(INT_23, SHORT_2, SHORT_3)
MK_INT(INT_24, SHORT_2, SHORT_4)
MK_INT(INT_34, SHORT_3, SHORT_4)

#undef MK_INT

#endif