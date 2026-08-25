// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Double byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc_io/dataset/0_double.hpp"
#include "minecraft/io/nbt/bytes.hpp" // IWYU pragma: keep
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Double>") {

  ByteParser<double> parser;

  //  --------------------------------------------------------------------------
  SUBCASE("[DOUBLE1] Normal case") {
    auto *p = DOUBLE1::BYTES;
    auto n = DOUBLE1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), DOUBLE1::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[DOUBLE2] Negative case") {
    auto *p = DOUBLE2::BYTES;
    auto n = DOUBLE2::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), DOUBLE2::VALUE);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[DOUBLE3] Big value case") {
    auto *p = DOUBLE3::BYTES;
    auto n = DOUBLE3::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), DOUBLE3::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_DOUBLE1] Two shorts") {
    auto *p = STREAM_DOUBLE1::STREAM;
    auto n = STREAM_DOUBLE1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_DOUBLE1::N_VALUES; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_DOUBLE1::VALUES[i]);
      CHECK_EQ(n, STREAM_DOUBLE1::N_BYTES - sizeof(double) * (i + 1));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_DOUBLE1] Incomplete stream") {
    auto *p = STREAM_DOUBLE1::STREAM;
    // We induce an incomplete stream
    auto n = STREAM_DOUBLE1::N_BYTES - 2;

    for (std::size_t i = 0; i < STREAM_DOUBLE1::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_DOUBLE1::VALUES[i]);
      CHECK_EQ(n, STREAM_DOUBLE1::N_BYTES - 2 - sizeof(double) * (i + 1));
    }

    // Incomplete parsing
    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::UNFINISHED);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
}