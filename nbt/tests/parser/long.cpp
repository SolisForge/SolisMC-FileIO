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

#include "minecraft/nbt/parser.hpp" // IWYU pragma: keep
#include "solismc_dataset/nbt/0_long.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Long>") {

  BytesParser<int64_t> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[LONG1] Normal case") {
    auto *p = static_cast<const StreamChar *>(LONG1::STREAM);
    auto n = LONG1::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), LONG1::VALUES[0]);
    CHECK_EQ(n, 0);
  }

  // --------------------------------------------------------------------------
  SUBCASE("[LONG2] Normal case") {
    auto *p = static_cast<const StreamChar *>(LONG2::STREAM);
    auto n = LONG2::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), LONG2::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_LONG] Parse of byte of negative value") {

    auto *p = static_cast<const StreamChar *>(NEGATIVE_LONG::STREAM);
    auto n = NEGATIVE_LONG::LENGTH;

    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_LT(VALUE_NEGATIVE_LONG::VALUE, 0);
    CHECK_EQ(parser.get(), VALUE_NEGATIVE_LONG::VALUE);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[MULTIPLE_LONGS] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(MULTIPLE_LONGS::STREAM);
    auto n = MULTIPLE_LONGS::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < MULTIPLE_LONGS::N_VALUES; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), MULTIPLE_LONGS::VALUES[i]);
      CHECK_EQ(n, MULTIPLE_LONGS::LENGTH - (i + 1) * sizeof(int64_t));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_LONGS] Not enough bytes") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_LONGS::STREAM);
    auto n = INCOMPLETE_LONGS::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < INCOMPLETE_LONGS::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INCOMPLETE_LONGS::VALUES[i]);
      CHECK_EQ(n, INCOMPLETE_LONGS::LENGTH - (i + 1) * sizeof(int64_t));
    }

    // Incomplete reading
    {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(),
               INCOMPLETE_LONGS::VALUES[INCOMPLETE_LONGS::N_VALUES - 1]);
      CHECK_EQ(n, 0);
    }
  }
}