// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Short byte parsing.
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

// Test resources
#include <common.hpp>
#include <integrals.hpp>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Short>") {

  BytesParser<Tags::Short> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[ONE_SHORT] Normal case") {
    parser.reset();

    uint8_t *p = (uint8_t *)(ONE_SHORT.STREAM);
    unsigned long n = ONE_SHORT.STREAM_LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), SHORT_1.value);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[TWO_SHORT] Several readings") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(TWO_SHORT.STREAM);
    unsigned long n = TWO_SHORT.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), SHORT_1.value);
      CHECK_EQ(n, TWO_SHORT.STREAM_LENGTH - 2);
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), SHORT_2.value);
      CHECK_EQ(n, TWO_SHORT.STREAM_LENGTH - 4);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_SHORTS] Not enough bytes") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(INCOMPLETE_THREE_SHORTS.STREAM);
    auto n = INCOMPLETE_THREE_SHORTS.STREAM_LENGTH;
    const auto N = INCOMPLETE_THREE_SHORTS.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), SHORT_1.value);
      CHECK_EQ(n, N - 2);
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), SHORT_2.value);
      CHECK_EQ(n, N - 4);
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
  SUBCASE("[NEGATIVE_SHORT] Parse of byte of negative value") {

    uint8_t *p = (uint8_t *)(NEGATIVE_SHORT.STREAM);
    auto n = NEGATIVE_SHORT.STREAM_LENGTH;

    parser.reset();
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(parser.get(), SHORT_2.value);
    CHECK_EQ(n, 0);
  }
}
