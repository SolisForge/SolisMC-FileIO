// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::String byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc_io/dataset/0_string.hpp"
#include "minecraft/io/nbt/bytes.hpp" // IWYU pragma: keep
#include <cstring>
#include <doctest/doctest.h>
#include <string>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::String>") {

  ByteParser<std::string> parser;

  // --------------------------------------------------------------------------
  SUBCASE("[STRING1] Normal case") {
    auto p = STRING1::BYTES;
    auto n = STRING1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    auto value = parser.get();
    CHECK_EQ(strcmp(value.c_str(), STRING1::VALUE), 0);
    CHECK_EQ(value.size(), STRING1::VALUE_LENGTH);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STRING2] Another normal case") {
    auto p = STRING2::BYTES;
    auto n = STRING2::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    auto value = parser.get();
    CHECK_EQ(strcmp(value.c_str(), STRING2::VALUE), 0);
    CHECK_EQ(value.size(), STRING2::VALUE_LENGTH);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STRING3] Long string case") {
    auto p = STRING3::BYTES;
    auto n = STRING3::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    auto value = parser.get();
    CHECK_EQ(strcmp(value.c_str(), STRING3::VALUE), 0);
    CHECK_EQ(value.size(), STRING3::VALUE_LENGTH);
    CHECK_EQ(n, 0);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STRING3] Unfinished case") {
    auto p = STRING3::BYTES;
    // We remove 10 bytes to simulate an unfinished stream
    auto n = STRING3::N_BYTES - 10;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::UNFINISHED);
    auto value = parser.get();
    CHECK_EQ(value.size(), 0);
    CHECK_EQ(n, 0);
  }
}