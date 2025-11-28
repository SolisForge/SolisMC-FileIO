// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Long byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parser/integral.hpp"
#include <cstdint>
#include <doctest/doctest.h>
#include <solismc/tests/nbt/integrals.hpp>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Long>") {

  BytesParser<Tags::Long> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[ONE_LONG] Normal case") {
    parser.reset();

    uint8_t *p = (uint8_t *)(ONE_LONG.STREAM);
    unsigned long n = ONE_LONG.STREAM_LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), LONG_1.value);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[TWO_LONG] Several readings") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(TWO_LONG.STREAM);
    unsigned long n = TWO_LONG.STREAM_LENGTH;
    const auto N = TWO_LONG.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), LONG_1.value);
      CHECK_EQ(n, N - sizeof(int64_t));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), LONG_3.value);
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_LONGS] Not enough bytes") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(INCOMPLETE_THREE_LONGS.STREAM);
    auto n = INCOMPLETE_THREE_LONGS.STREAM_LENGTH;
    const auto N = INCOMPLETE_THREE_LONGS.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), LONG_1.value);
      CHECK_EQ(n, N - sizeof(int64_t));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), LONG_3.value);
      CHECK_EQ(n, N - 2 * sizeof(int64_t));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), BYTE_3.value);
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_LONG] Parse of byte of negative value") {

    uint8_t *p = (uint8_t *)(NEGATIVE_LONG.STREAM);
    auto n = NEGATIVE_LONG.STREAM_LENGTH;

    parser.reset();
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(parser.get(), LONG_2.value);
    CHECK_EQ(n, 0);
  }
}
