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
#include "solismc_dataset/nbt/0_int.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Int>") {

  BytesParser<int32_t> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[INT1] Normal case") {
    auto *p = static_cast<const StreamChar *>(INT1::STREAM);
    auto n = INT1::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), INT1::VALUES[0]);
    CHECK_EQ(n, 0);
  }

  // --------------------------------------------------------------------------
  SUBCASE("[INT2] Normal case") {
    auto *p = static_cast<const StreamChar *>(INT2::STREAM);
    auto n = INT2::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), INT2::VALUES[0]);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[NEGATIVE_INT] Parse of byte of negative value") {

    auto *p = static_cast<const StreamChar *>(NEGATIVE_INT::STREAM);
    auto n = NEGATIVE_INT::LENGTH;

    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_LT(parser.get(), 0);
    CHECK_LT(VALUE_NEGATIVE_INT::VALUE, 0);
    CHECK_EQ(parser.get(), VALUE_NEGATIVE_INT::VALUE);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[MULTIPLE_INTS] Several readings") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(MULTIPLE_INTS::STREAM);
    auto n = MULTIPLE_INTS::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < MULTIPLE_INTS::N_VALUES; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), MULTIPLE_INTS::VALUES[i]);
      CHECK_EQ(n, MULTIPLE_INTS::LENGTH - (i + 1) * sizeof(int32_t));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[INCOMPLETE_THREE_INTS] Not enough bytes") {
    // Base input buffer
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_INTS::STREAM);
    auto n = INCOMPLETE_INTS::LENGTH;

    // Read all values
    for (unsigned long i = 0; i < INCOMPLETE_INTS::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), INCOMPLETE_INTS::VALUES[i]);
      CHECK_EQ(n, INCOMPLETE_INTS::LENGTH - (i + 1) * sizeof(int32_t));
    }

    // Incomplete reading
    {
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(),
               INCOMPLETE_INTS::VALUES[INCOMPLETE_INTS::N_VALUES - 1]);
      CHECK_EQ(n, 0);
    }
  }
}