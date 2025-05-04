#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_array.hpp>
#include <minecraft/nbt/tests/tests.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::ByteArray parsing") {
  // Input
  auto parser = NBTByteArrayParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  Expected<ByteArray> exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[15]{
        NBTTags::ByteArray,
        '\x04',
        '\x00',
        'B',
        'A',
        'r',
        'r',
        '\x04',
        '\x00',
        '\x00',
        '\x00',
        '\x01',
        (uint8_t)'\x80',
        (uint8_t)'\x65',
        (uint8_t)'\xe7',
    };
    N = 15;
    exp = Expected<ByteArray>{{ParseResult::SUCCESS, 4, "BArr"},
                              4,
                              {1, (int8_t)0x80, 0x65, (int8_t)0xe7}};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[18]{NBTTags::ByteArray,
                            '\x04',
                            '\x00',
                            'B',
                            'A',
                            'r',
                            'r',
                            '\x04',
                            '\x00',
                            '\x00',
                            '\x00',
                            '\x01',
                            (uint8_t)'\x80',
                            (uint8_t)'\x65',
                            (uint8_t)'\xe7',
                            '\x11',
                            '\x11',
                            '\x11'};
    N = 18;
    exp = Expected<ByteArray>{{ParseResult::SUCCESS, 4, "BArr"},
                              4,
                              {1, (int8_t)0x80, 0x65, (int8_t)0xe7}};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{
        NBTTags::ByteArray, '\x04', '\x00', 'B', 'A', 'r',
    };
    N = 6;
    exp = Expected<ByteArray>{{ParseResult::UNFINISHED, 4, "BAr"}, 0, {}};
  }

  // Test the case
  uint8_t *p = bytes;
  auto ret = parser.parse(&p, N);
  auto b = parser.getParsed();
  exp.compare_to(ret, b);
  if (bytes != nullptr)
    delete bytes;
  delete b;
}