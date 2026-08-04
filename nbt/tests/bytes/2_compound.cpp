// ============================================================================
// Project: SOLISMC-FILEIO
//
// Unit-test for compound
//
// Author    Meltwin (github@meltwin.fr)
// Date      15/07/2026 (created 15/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes.hpp"
#include "minecraft/io/nbt/compound.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt::byte::base;
using namespace minecraft::nbt;

TEST_CASE("ByteParser<Compound>") {
  minecraft::nbt::ByteParser<minecraft::nbt::Compound> parser{};

  // Normal case
  SUBCASE("Normal parsing of whole object") {
    const char stream[28]{(int8_t)Tag::INT, // Int value
                          '\x00',
                          '\x03',
                          'f',
                          'o',
                          'o',
                          '\x00',
                          '\x00',
                          '\x00',
                          '\x0f',
                          (int8_t)Tag::STRING,
                          '\x00',
                          '\x04',
                          'n',
                          'a',
                          'm',
                          'e',
                          '\x00',
                          '\x08',
                          't',
                          'e',
                          's',
                          't',
                          'n',
                          'a',
                          'm',
                          'e',
                          (int8_t)Tag::END};
    auto p = stream;
    size_t n = 28;

    auto ret = parser.parse(p, n);
    auto obj = parser.get();

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(n, 0);

    // Verify parsed value
    auto name = obj.get("name").as<std::string>();
    CHECK_EQ(name.size(), 8);
    CHECK_EQ(name.compare("testname"), 0);
    CHECK_EQ(obj.get("foo").as<int>(), 15);
  }

  // Partial case
  SUBCASE("Parsing of partial object") {
    const char stream[11]{(int8_t)Tag::INT,
                          '\x00',
                          '\x03',
                          'f',
                          'o',
                          'o',
                          '\x00',
                          '\x00',
                          '\x00',
                          '\x0f',
                          (int8_t)Tag::END};
    auto p = stream;
    size_t n = 11;

    auto ret = parser.parse(p, n);
    auto obj = parser.get();

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(n, 0);

    // Verify parsed value
    CHECK_THROWS(obj.get("name"));
  }
}