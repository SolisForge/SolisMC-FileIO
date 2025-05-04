#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_array.hpp>
#include <minecraft/nbt/tests/tests.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::ByteArray parsing") {
  // Input
  auto parser = NBTLongArrayParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  Expected<LongArray> exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[19]{
        NBTTags::LongArray,
        '\x04',
        '\x00',
        'L',
        'A',
        'r',
        'r',
        '\x01',
        '\x00',
        '\x00',
        '\x00',
        '\x01',
        (uint8_t)'\x80',
        (uint8_t)'\x65',
        (uint8_t)'\xe7',
        '\x02',
        (uint8_t)'\xa9',
        (uint8_t)'\x02',
        (uint8_t)'\xd6',
    };
    N = 19;
    exp = Expected<LongArray>{
        {ParseResult::SUCCESS, 4, "LArr"},
        1,
        {solis::FROM_BIG_ENDIAN<int64_t>(0x018065e702a902d6)}};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[30]{NBTTags::LongArray,
                            '\x04',
                            '\x00',
                            'L',
                            'A',
                            'r',
                            'r',
                            '\x02',
                            '\x00',
                            '\x00',
                            '\x00',
                            '\x01',
                            (uint8_t)'\x80',
                            (uint8_t)'\x65',
                            (uint8_t)'\xe7',
                            '\x02',
                            (uint8_t)'\xa9',
                            (uint8_t)'\x02',
                            (uint8_t)'\xd6',
                            '\x01',
                            (uint8_t)'\x80',
                            (uint8_t)'\x65',
                            (uint8_t)'\xe7',
                            '\x02',
                            (uint8_t)'\xa9',
                            (uint8_t)'\x02',
                            (uint8_t)'\xd6',
                            '\x11',
                            '\x11',
                            '\x11'};
    N = 30;
    exp = Expected<LongArray>{
        {ParseResult::SUCCESS, 4, "LArr"},
        2,
        {solis::FROM_BIG_ENDIAN<int64_t>(0x018065e702a902d6),
         solis::FROM_BIG_ENDIAN<int64_t>(0x018065e702a902d6)}};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{
        NBTTags::LongArray, '\x04', '\x00', 'L', 'A', 'r',
    };
    N = 6;
    exp = Expected<LongArray>{{ParseResult::UNFINISHED, 4, "LAr"}, 0, {}};
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