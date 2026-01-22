// ============================================================================
// Project: SOLISMC-FILEIO
//
// Unittests for NBT::String byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      05/01/2026 (created 05/01/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parsers/string.hpp"
#include "solismc_dataset/nbt/1_string.hpp"
#include <cstdint>
#include <doctest/doctest.h>

using namespace minecraft::nbt;

#define CHECK_PARSED_STR(set, result, index, remaining)                        \
  CHECK_EQ(ret, ParseResult::result);                                          \
  CHECK_EQ(parser.get_length(), set::VALUES_LENGTH[index]);                    \
  CHECK_EQ(parser.get().size(), set::VALUES_LENGTH[index]);                    \
  CHECK_EQ(parser.get(), set::VALUES[index]);                                  \
  CHECK_EQ(n, remaining)

// ============================================================================
TEST_CASE("BytesParser<NBT::String>") {
  BytesParser<std::string> parser;

  SUBCASE("[SHORT_STR] Normal case") {
    auto *p = static_cast<const StreamChar *>(SHORT_STR::STREAM);
    auto n = SHORT_STR::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_PARSED_STR(SHORT_STR, SUCCESS, 0, 0);
  }
  SUBCASE("[LONG_STR] Normal case") {
    auto *p = static_cast<const StreamChar *>(LONG_STR::STREAM);
    auto n = LONG_STR::LENGTH;
    auto ret = parser.parse(p, n);

    CHECK_PARSED_STR(LONG_STR, SUCCESS, 0, 0);
  }
  SUBCASE("[MULTIPLE_STRS] Multiple strings") {
    auto *p = static_cast<const StreamChar *>(MULTIPLE_STRS::STREAM);
    auto n = MULTIPLE_STRS::LENGTH;
    auto REMAINING = MULTIPLE_STRS::LENGTH;

    for (std::size_t i = 0; i < MULTIPLE_STRS::N_VALUES; i++) {
      auto ret = parser.parse(p, n);

      REMAINING -= MULTIPLE_STRS::VALUES_LENGTH[i] + 2;

      CHECK_PARSED_STR(MULTIPLE_STRS, SUCCESS, i, REMAINING);
    }
  }
  SUBCASE("[INCOMPLETE_STRS] Incomplete multiple strings") {
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_STRS::STREAM);
    auto n = INCOMPLETE_STRS::LENGTH;
    auto REMAINING = INCOMPLETE_STRS::LENGTH;

    for (std::size_t i = 0; i < INCOMPLETE_STRS::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);

      REMAINING -= INCOMPLETE_STRS::VALUES_LENGTH[i] + sizeof(uint16_t);

      CHECK_PARSED_STR(INCOMPLETE_STRS, SUCCESS, i, REMAINING);
    }

    // Check incomplete
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::UNFINISHED);
    CHECK_EQ(parser.get_length(),
             INCOMPLETE_STRS::VALUES_LENGTH[INCOMPLETE_STRS::N_VALUES - 1]);
    CHECK_EQ(parser.get().size(), 0);
    CHECK_EQ(parser.get(), "");
    CHECK_EQ(n, 0);
  }
}