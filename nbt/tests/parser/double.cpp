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

#include "minecraft/nbt/parser.hpp" // IWYU pragma: keep
#include "solismc_dataset/nbt/1_double.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Double>") {
  BytesParser<double> parser;

  //   --------------------------------------------------------------------------
  SUBCASE("[DOUBLE1] Normal case") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(DOUBLE1::STREAM);
    auto n = DOUBLE1::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), DOUBLE1::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  //   --------------------------------------------------------------------------
  SUBCASE("[DOUBLE2] Normal case") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(DOUBLE2::STREAM);
    auto n = DOUBLE2::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), DOUBLE2::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  //   --------------------------------------------------------------------------
  SUBCASE("[BIG_DOUBLE] Big DOUBLE") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(BIG_DOUBLE::STREAM);
    auto n = BIG_DOUBLE::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), BIG_DOUBLE::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  //   --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_DOUBLE] Negative DOUBLE") {
    parser.reset();

    auto *p = static_cast<const StreamChar *>(NEGATIVE_DOUBLE::STREAM);
    auto n = NEGATIVE_DOUBLE::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(NEGATIVE_DOUBLE::VALUES[0], 0);
    CHECK_EQ(parser.get(), NEGATIVE_DOUBLE::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[MULTIPLE_DOUBLES] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(MULTIPLE_DOUBLES::STREAM);
    auto n = MULTIPLE_DOUBLES::LENGTH;

    for (unsigned long i = 0; i < MULTIPLE_DOUBLES::N_VALUES; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), MULTIPLE_DOUBLES::VALUES[i]);
      CHECK_EQ(n, MULTIPLE_DOUBLES::LENGTH - (i + 1) * sizeof(double));
    }
    CHECK_EQ(n, 0);
  }
  // ----------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_DOUBLES] Not enough bytes") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_DOUBLES::STREAM);
    auto n = INCOMPLETE_DOUBLES::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < INCOMPLETE_DOUBLES::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INCOMPLETE_DOUBLES::VALUES[i]);
      CHECK_EQ(n, INCOMPLETE_DOUBLES::LENGTH - (i + 1) * sizeof(double));
    }

    // Incomplete reading
    {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(),
               INCOMPLETE_DOUBLES::VALUES[INCOMPLETE_DOUBLES::N_VALUES - 1]);
      CHECK_EQ(n, 0);
    }
  }
}
