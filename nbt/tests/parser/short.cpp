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

#include "minecraft/nbt/parser.hpp" // IWYU pragma: keep
#include "solismc/tests/nbt/integrals.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Short>") {

  BytesParser<int16_t> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[ONE_SHORT] Normal case") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(ONE_SHORT.STREAM);
    auto n = ONE_SHORT.STREAM_LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), SHORT_1.value);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[TWO_SHORT] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(TWO_SHORT.STREAM);
    auto n = TWO_SHORT.STREAM_LENGTH;

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
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_THREE_SHORTS.STREAM);
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

    // Third reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), 0);
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_SHORT] Parse of byte of negative value") {

    auto *p = static_cast<const StreamChar *>(NEGATIVE_SHORT.STREAM);
    auto n = NEGATIVE_SHORT.STREAM_LENGTH;

    parser.reset();
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_LT(SHORT_4.value, 0);
    CHECK_EQ(parser.get(), SHORT_4.value);
    CHECK_EQ(n, 0);
  }
}
