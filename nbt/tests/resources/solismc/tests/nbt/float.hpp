// ============================================================================
// Project: SOLISMC_FILEIO
//
// This file contains test values for NBT floating point parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      29/11/2025 (created 29/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#ifndef SOLIS_MC_NBT_FLOATING_POINT_TEST_SET_HPP
#define SOLIS_MC_NBT_FLOATING_POINT_TEST_SET_HPP

#include <bit>
#include <solismc/tests/nbt/common.hpp>
#include <solismc/tests/nbt/integrals.hpp>

/**
 * @brief Structure to represent the bytes representation of a float value.
 *
 * @tparam T the output type
 * @tparam Bytes... the bytes representation (big-endian)
 */
template <typename T, uint8_t... Bytes>
struct FloatValue : solis::NBTValue<T, Bytes...> {

  explicit constexpr FloatValue<T, Bytes...>(const T &value)
      : solis::NBTValue<T, Bytes...>(value) {}
};

/**
 * @brief Create a floating point value based from an integral value.
 *
 * @tparam T The floating point type
 * @tparam Ti The integral type
 * @tparam Bytes... the bytes representation (big-endian)
 * @param fval the float value
 */
template <typename T, typename Ti, uint8_t... Bytes>
constexpr FloatValue<T, Bytes...>
float_from_int(IntegralValue<Ti, Bytes...> ival) {
  static_assert(sizeof(T) == sizeof(Ti),
                "The sizes of the float and integral types must be the same");
  return FloatValue<T, Bytes...>(std::bit_cast<T>(ival.value));
}

// ============================================================================
// Floats
// ============================================================================

#define FLOAT_FROM_INT(name, i_val)                                            \
  [[maybe_unused]] constexpr auto FLOAT_##name = float_from_int<float>(i_val);

FLOAT_FROM_INT(1, INT_1);
FLOAT_FROM_INT(2, INT_2);

#undef FLOAT_FROM_INT

constexpr auto ONE_FLOAT = solis::NBTstream(FLOAT_1);
constexpr auto TWO_FLOAT = ONE_FLOAT + FLOAT_2;
constexpr auto INCOMPLETE_THREE_FLOATS = TWO_FLOAT + BYTE_1;

// ============================================================================
// Double
// ============================================================================

#define DOUBLE_FROM_LONG(name, i_val)                                          \
  [[maybe_unused]] constexpr auto DOUBLE_##name = float_from_int<double>(i_val);

DOUBLE_FROM_LONG(1, LONG_1);
DOUBLE_FROM_LONG(2, LONG_2);

#undef DOUBLE_FROM_LONG

constexpr auto ONE_DOUBLE = solis::NBTstream(DOUBLE_1);
constexpr auto TWO_DOUBLE = ONE_DOUBLE + DOUBLE_2;
constexpr auto INCOMPLETE_THREE_DOUBLES = TWO_DOUBLE + BYTE_1;

#endif