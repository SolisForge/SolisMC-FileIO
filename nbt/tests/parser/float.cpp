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

#include "minecraft/nbt/parser.hpp" // IWYU pragma: keep
#include "solismc_dataset/nbt/1_float.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Float>") {
  BytesParser<float> parser;

  //   --------------------------------------------------------------------------
  SUBCASE("[FLOAT1] Normal case") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(FLOAT1::STREAM);
    auto n = FLOAT1::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), FLOAT1::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  //   --------------------------------------------------------------------------
  SUBCASE("[FLOAT2] Normal case") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(FLOAT2::STREAM);
    auto n = FLOAT2::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), FLOAT2::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  //   --------------------------------------------------------------------------
  SUBCASE("[BIG_FLOAT] Big float") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(BIG_FLOAT::STREAM);
    auto n = BIG_FLOAT::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), BIG_FLOAT::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  //   --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_FLOAT] Negative float") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(NEGATIVE_FLOAT::STREAM);
    auto n = NEGATIVE_FLOAT::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(NEGATIVE_FLOAT::VALUES[0], 0);
    CHECK_EQ(parser.get(), NEGATIVE_FLOAT::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[MULTIPLE_FLOATS] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(MULTIPLE_FLOATS::STREAM);
    auto n = MULTIPLE_FLOATS::LENGTH;

    for (unsigned long i = 0; i < MULTIPLE_FLOATS::N_VALUES; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), MULTIPLE_FLOATS::VALUES[i]);
      CHECK_EQ(n, MULTIPLE_FLOATS::LENGTH - (i + 1) * sizeof(float));
    }
    CHECK_EQ(n, 0);
  }
  // ----------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_FLOATS] Not enough bytes") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_FLOATS::STREAM);
    auto n = INCOMPLETE_FLOATS::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < INCOMPLETE_FLOATS::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INCOMPLETE_FLOATS::VALUES[i]);
      CHECK_EQ(n, INCOMPLETE_FLOATS::LENGTH - (i + 1) * sizeof(float));
    }

    // Incomplete reading
    {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(),
               INCOMPLETE_FLOATS::VALUES[INCOMPLETE_FLOATS::N_VALUES - 1]);
      CHECK_EQ(n, 0);
    }
  }
}
