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
#include "solismc_dataset/nbt/0_short.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Short>") {

  BytesParser<int16_t> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[SHORT1] Normal case") {
    auto *p = static_cast<const StreamChar *>(SHORT1::STREAM);
    auto n = SHORT1::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), SHORT1::VALUES[0]);
    CHECK_EQ(n, 0);
  }

  // --------------------------------------------------------------------------
  SUBCASE("[SHORT2] Normal case") {
    auto *p = static_cast<const StreamChar *>(SHORT2::STREAM);
    auto n = SHORT2::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), SHORT2::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_SHORT] Parse of byte of negative value") {

    auto *p = static_cast<const StreamChar *>(NEGATIVE_SHORT::STREAM);
    auto n = NEGATIVE_SHORT::LENGTH;

    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_LT(VALUE_NEGATIVE_SHORT::VALUE, 0);
    CHECK_EQ(parser.get(), VALUE_NEGATIVE_SHORT::VALUE);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[MULTIPLE_SHORTS] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(MULTIPLE_SHORTS::STREAM);
    auto n = MULTIPLE_SHORTS::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < MULTIPLE_SHORTS::N_VALUES; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), MULTIPLE_SHORTS::VALUES[i]);
      CHECK_EQ(n, MULTIPLE_SHORTS::LENGTH - (i + 1) * sizeof(int16_t));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_SHORTS] Not enough bytes") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_SHORTS::STREAM);
    auto n = INCOMPLETE_SHORTS::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < INCOMPLETE_SHORTS::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INCOMPLETE_SHORTS::VALUES[i]);
      CHECK_EQ(n, INCOMPLETE_SHORTS::LENGTH - (i + 1) * sizeof(int16_t));
    }

    // Incomplete reading
    {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(),
               INCOMPLETE_SHORTS::VALUES[INCOMPLETE_SHORTS::N_VALUES - 1]);
      CHECK_EQ(n, 0);
    }
  }
}