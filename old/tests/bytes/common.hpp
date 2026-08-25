// ============================================================================
// Project: SOLISMC-FILEIO
//
// Common functions & macros for tests
//
// Author    Meltwin (github@meltwin.fr)
// Date      19/06/2026 (created 19/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_BYTE_UNITTESTS
#define SOLISMC_IO_NBT_BYTE_UNITTESTS

#include "minecraft/io/nbt/bytes/interface.hpp"
#include <array>
#include <doctest/doctest.h>

using minecraft::nbt::ParseResult;
using minecraft::nbt::Size;
using minecraft::nbt::Stream;
using minecraft::nbt::WriteResult;

inline void buffers_are_equal(const Stream &a, const Stream &b, short n) {
  for (short i = 0; i < n; i++) {
    CHECK_EQ(a[i], b[i]);
  }
}
template <std::size_t N>
inline void buffers_are_equal(const std::array<char, N> &a, const Stream &b,
                              short n) {
  for (short i = 0; i < n; i++) {
    CHECK_EQ(a[i], b[i]);
  }
}

inline void check_parsing_ended(const ParseResult ret, const Size &n) {
  CHECK_EQ(ret, ParseResult::ENDED);
  CHECK_EQ(n, 0);
}
inline void check_writing_ended(const WriteResult ret, const Size &n) {
  CHECK_EQ(ret, WriteResult::ENDED);
  CHECK_EQ(n, 0);
}

#endif