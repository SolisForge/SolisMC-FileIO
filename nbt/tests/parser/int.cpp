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
TEST_CASE("BytesParser<NBT::Int>") {

  BytesParser<Tags::Int> parser;

  // --------------------------------------------------------------------------
  SUBCASE("Normal case") {
    int8_t input[4]{'\x16', '\x59', '\x33', '\x86'};
    unsigned long N = 4;
    parser.reset();

    uint8_t *p = reinterpret_cast<uint8_t *>(input);
    unsigned long n = N;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), 0x86335916);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("Bytes greater than max signed int value") {
    char input[4]{'\x00', '\x00', '\x00', '\x90'};
    unsigned long N = 4;
    parser.reset();

    uint8_t *p = reinterpret_cast<unsigned char *>(&input);
    unsigned long n = N;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get(), static_cast<int32_t>(0x90000000));
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("Not enough bytes") {
    SUBCASE("1 byte") {
      char input[1]{'\x16'};
      unsigned long N = 1;
      parser.reset();

      uint8_t *p = reinterpret_cast<unsigned char *>(&input);
      unsigned long n = N;
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), 0x00000016);
      CHECK_EQ(n, 0);
    }
    SUBCASE("2 byte") {
      char input[2]{'\x16', '\x33'};
      unsigned long N = 2;
      parser.reset();

      uint8_t *p = reinterpret_cast<unsigned char *>(&input);
      unsigned long n = N;
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), 0x00003316);
      CHECK_EQ(n, 0);
    }
    SUBCASE("3 byte") {
      char input[3]{'\x43', '\x16', '\x33'};
      unsigned long N = 3;
      parser.reset();

      uint8_t *p = reinterpret_cast<unsigned char *>(&input);
      unsigned long n = N;
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), 0x00331643);
      CHECK_EQ(n, 0);
    }
  }
  // --------------------------------------------------------------------------
  SUBCASE("Several readings") {
    // Base input buffer
    char input[9]{'\x16', '\x59', '\x32', '\x17', '\x84',
                  '\x16', '\x59', '\x32', '\x17'};
    unsigned long N = 9;

    // First reading
    uint8_t *p = reinterpret_cast<unsigned char *>(&input);
    unsigned long n = N;
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), 0x17325916);
      CHECK_EQ(n, 5);
    }

    // Second reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::SUCCESS);
      CHECK_EQ(parser.get(), 0x32591684);
      CHECK_EQ(n, 1);
    }

    // Third reading
    {
      parser.reset();
      auto ret = parser.parse(p, n);

      CHECK_EQ(ret, ParseResult::UNFINISHED);
      CHECK_EQ(parser.get(), static_cast<int32_t>(0x00000017));
      CHECK_EQ(n, 0);
    }
  }
}
