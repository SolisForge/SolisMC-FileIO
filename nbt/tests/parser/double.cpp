// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Double byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      29/11/2025 (created 29/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parser/float.hpp"
#include "solismc/tests/nbt/float.hpp"
#include "solismc/tests/nbt/integrals.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Double>") {

  BytesParser<Tags::Double> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[ONE_DOUBLE] Normal case") {
    parser.reset();

    uint8_t *p = (uint8_t *)(ONE_DOUBLE.STREAM);
    unsigned long n = ONE_DOUBLE.STREAM_LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), DOUBLE_1.value);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[TWO_DOUBLE] Several readings") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(TWO_DOUBLE.STREAM);
    unsigned long n = TWO_DOUBLE.STREAM_LENGTH;
    const auto N = TWO_DOUBLE.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), DOUBLE_1.value);
      CHECK_EQ(n, N - sizeof(double));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), DOUBLE_2.value);
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_DOUBLES] Not enough bytes") {
    // Base input buffer
    uint8_t *p = (uint8_t *)(INCOMPLETE_THREE_DOUBLES.STREAM);
    auto n = INCOMPLETE_THREE_DOUBLES.STREAM_LENGTH;
    const auto N = INCOMPLETE_THREE_DOUBLES.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), DOUBLE_1.value);
      CHECK_EQ(n, N - sizeof(double));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), DOUBLE_2.value);
      CHECK_EQ(n, N - 2 * sizeof(double));
    }

    // Third reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), 0);
      CHECK_EQ(n, 0);
    }
  }
}
