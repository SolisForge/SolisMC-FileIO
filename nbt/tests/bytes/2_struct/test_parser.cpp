// ============================================================================
// Project: SOLISMC-FILEIO
//
// Unit-test for C++ struct parser
//
// Author    Meltwin (github@meltwin.fr)
// Date      15/07/2026 (created 15/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "adapter.hpp"
#include "minecraft/io/nbt/bytes/interface.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt::byte::base;
using namespace minecraft::nbt;

TEST_CASE("ByteParser<C++ Object>") {
  auto obj = std::make_shared<TestObject>(std::string{}, 0);
  TestObjectWriteAdapter adapter{obj};
  ObjectParser<TestObjectWriteAdapter, GameVersion::JAVA> parser{adapter};

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

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(n, 0);

    // Verify parsed value
    CHECK_EQ(obj->name.size(), 8);
    CHECK_EQ(obj->name.compare("testname"), 0);
    CHECK_EQ(obj->foo, 15);
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

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(n, 0);

    // Verify parsed value
    CHECK_EQ(obj->name.size(), 0);
    CHECK_EQ(obj->name.compare(""), 0);
    CHECK_EQ(obj->foo, 15);
  }
}