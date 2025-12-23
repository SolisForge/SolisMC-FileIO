// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Float byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      29/11/2025 (created 29/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc/tests/nbt/float.hpp"
#include "minecraft/nbt/parser.hpp" // IWYU pragma: keep
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Float>") {
  BytesParser<Tags::Float> parser;

  //   --------------------------------------------------------------------------
  SUBCASE("[ONE_FLOAT] Normal case") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(ONE_FLOAT.STREAM);
    auto n = ONE_FLOAT.STREAM_LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), FLOAT_1.value);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[TWO_FLOAT] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(TWO_FLOAT.STREAM);
    auto n = TWO_FLOAT.STREAM_LENGTH;
    const auto N = TWO_FLOAT.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), FLOAT_1.value);
      CHECK_EQ(n, N - sizeof(float));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), FLOAT_2.value);
      CHECK_EQ(n, 0);
    }
  }
  // ----------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_FLOATS] Not enough bytes") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_THREE_FLOATS.STREAM);
    auto n = INCOMPLETE_THREE_FLOATS.STREAM_LENGTH;
    const auto N = INCOMPLETE_THREE_FLOATS.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), FLOAT_1.value);
      CHECK_EQ(n, N - sizeof(float));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), FLOAT_2.value);
      CHECK_EQ(n, N - 2 * sizeof(float));
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
