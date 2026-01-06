// ============================================================================
// Project: SOLISMC-FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      05/01/2026 (created 05/01/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLIS_MC_NBT_STRING_TEST_SET_HPP
#define SOLIS_MC_NBT_STRING_TEST_SET_HPP

#include <solismc/tests/nbt/common.hpp>
#include <string>

/**
 * @brief Structure to store string constants for unittests
 *
 * @tparam Bytes the bytes to be parsed (including the size bytes)
 */
template <solis::StreamChar... Bytes>
struct StringValue : solis::NBTValue<std::string, Bytes...> {

  explicit constexpr StringValue<Bytes...>(const std::string value)
      : solis::NBTValue<std::string, Bytes...>(value) {}
};

// ============================================================================
// String constants
// ============================================================================
#define MK_STRING(name, str_val, ...)                                          \
  [[maybe_unused]] constexpr auto STRING_RAW_LENGTH_##name =                   \
      sizeof(str_val) - 1;                                                     \
  [[maybe_unused]] constexpr auto STRING_##name = StringValue<                 \
      static_cast<solis::StreamChar>(STRING_RAW_LENGTH_##name >> 8),           \
      static_cast<solis::StreamChar>(STRING_RAW_LENGTH_##name & 0xff),         \
      __VA_ARGS__>(str_val);

MK_STRING(ONE, "Foobar", '\x46', '\x6f', '\x6f', '\x62', '\x61', '\x72')

#undef MK_STRING

#endif