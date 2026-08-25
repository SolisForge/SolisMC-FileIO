// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Float byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc_io/dataset/0_float.hpp"
#include "minecraft/io/nbt/bytes.hpp" // IWYU pragma: keep
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Float>") {

  ByteParser<float> parser;

  //  --------------------------------------------------------------------------
  SUBCASE("[FLOAT1] Normal case") {
    auto *p = FLOAT1::BYTES;
    auto n = FLOAT1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), FLOAT1::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[FLOAT2] Negative case") {
    auto *p = FLOAT2::BYTES;
    auto n = FLOAT2::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), FLOAT2::VALUE);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[FLOAT3] Big value case") {
    auto *p = FLOAT3::BYTES;
    auto n = FLOAT3::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), FLOAT3::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_FLOAT1] Two shorts") {
    auto *p = STREAM_FLOAT1::STREAM;
    auto n = STREAM_FLOAT1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_FLOAT1::N_VALUES; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_FLOAT1::VALUES[i]);
      CHECK_EQ(n, STREAM_FLOAT1::N_BYTES - sizeof(float) * (i + 1));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_FLOAT1] Incomplete stream") {
    auto *p = STREAM_FLOAT1::STREAM;
    // We induce an incomplete stream
    auto n = STREAM_FLOAT1::N_BYTES - 2;

    for (std::size_t i = 0; i < STREAM_FLOAT1::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_FLOAT1::VALUES[i]);
      CHECK_EQ(n, STREAM_FLOAT1::N_BYTES - 2 - sizeof(float) * (i + 1));
    }

    // Incomplete parsing
    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::UNFINISHED);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
}