// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Int byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parser.hpp" // IWYU pragma: keep
#include "solismc/tests/nbt/integrals.hpp"
#include <cstdint>
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Int>") {

  BytesParser<Tags::Int> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[ONE_INT] Normal case") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(ONE_INT.STREAM);
    auto n = ONE_INT.STREAM_LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), INT_2.value);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[TWO_INT] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(TWO_INT.STREAM);
    auto n = TWO_INT.STREAM_LENGTH;
    const auto N = TWO_INT.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_2.value);
      CHECK_EQ(n, N - sizeof(int32_t));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_3.value);
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_INTS] Not enough bytes") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_THREE_INTS.STREAM);
    auto n = INCOMPLETE_THREE_INTS.STREAM_LENGTH;
    const auto N = INCOMPLETE_THREE_INTS.STREAM_LENGTH;

    // First reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_2.value);
      CHECK_EQ(n, N - sizeof(int32_t));
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INT_3.value);
      CHECK_EQ(n, N - 2 * sizeof(int32_t));
    }

    // Third reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), BYTE_3.value << 8 * (sizeof(int32_t) - 1));
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_INT] Parse of byte of negative value") {

    auto *p = static_cast<const StreamChar *>(NEGATIVE_INT.STREAM);
    auto n = NEGATIVE_INT.STREAM_LENGTH;

    parser.reset();
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_LT(INT_3.value, 0);
    CHECK_EQ(parser.get(), INT_3.value);
    CHECK_EQ(n, 0);
  }
}
