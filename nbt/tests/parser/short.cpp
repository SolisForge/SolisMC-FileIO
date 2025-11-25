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

#include "minecraft/nbt/parser/integral.hpp"
#include <cstdint>
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Short>") {

  BytesParser<Tags::Short> parser;

  // --------------------------------------------------------------------------
  SUBCASE("Normal case") {
    int8_t input[2]{'\x16', '\x59'};
    unsigned long N = 2;
    parser.reset();

    uint8_t *p = reinterpret_cast<uint8_t *>(input);
    unsigned long n = N;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), 0x5916);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("Bytes greater than max signed short value") {
    char input[2]{'\x00', '\x90'};
    unsigned long N = 2;
    parser.reset();

    uint8_t *p = reinterpret_cast<unsigned char *>(&input);
    unsigned long n = N;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), static_cast<int16_t>(0x9000));
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("Not enough bytes") {
    char input[1]{'\x16'};
    unsigned long N = 1;
    parser.reset();

    uint8_t *p = reinterpret_cast<unsigned char *>(&input);
    unsigned long n = N;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::UNFINISHED);
    CHECK_EQ(parser.get(), 0x0016);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("Several readings") {
    // Base input buffer
    char input[5]{'\x16', '\x59', '\x32', '\x17', '\x84'};
    unsigned long N = 5;

    // First reading
    uint8_t *p = reinterpret_cast<unsigned char *>(&input);
    unsigned long n = N;
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), 0x5916);
      CHECK_EQ(n, N - 2);
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), 0x1732);
      CHECK_EQ(n, 1);
    }

    // Third reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), static_cast<int16_t>(0x0084));
      CHECK_EQ(n, 0);
    }
  }
}
