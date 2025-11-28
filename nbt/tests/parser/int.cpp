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
#include <solismc/tests/nbt/integrals.hpp>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Int>") {

  BytesParser<Tags::Int> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[ONE_INT] Normal case") {
    parser.reset();

    uint8_t *p = (uint8_t *)(ONE_INT.STREAM);
    unsigned long n = ONE_INT.STREAM_LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), INT_23.value);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[TWO_INT] Several readings") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(TWO_INT.STREAM);
    unsigned long n = TWO_INT.STREAM_LENGTH;
    const auto N = TWO_INT.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_23.value);
      CHECK_EQ(n, N - sizeof(int32_t));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_34.value);
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_INTS] Not enough bytes") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(INCOMPLETE_THREE_INTS.STREAM);
    auto n = INCOMPLETE_THREE_INTS.STREAM_LENGTH;
    const auto N = INCOMPLETE_THREE_INTS.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_23.value);
      CHECK_EQ(n, N - sizeof(int32_t));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_34.value);
      CHECK_EQ(n, N - 2 * sizeof(int32_t));
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
  SUBCASE("[NEGATIVE_INT] Parse of byte of negative value") {

    uint8_t *p = (uint8_t *)(NEGATIVE_INT.STREAM);
    auto n = NEGATIVE_INT.STREAM_LENGTH;

    parser.reset();
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(parser.get(), INT_12.value);
    CHECK_EQ(n, 0);
  }
}
