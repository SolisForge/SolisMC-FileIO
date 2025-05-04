#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_array.hpp>
#include <minecraft/nbt/tests/tests.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::IntArray parsing") {
  // Input
  auto parser = NBTIntArrayParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  Expected<IntArray> exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[19]{
        NBTTags::IntArray,
        '\x04',
        '\x00',
        'I',
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
    };
    N = 19;
    exp = Expected<IntArray>{{ParseResult::SUCCESS, 4, "IArr"},
                             2,
                             {solis::FROM_BIG_ENDIAN<int32_t>(0x018065e7),
                              solis::FROM_BIG_ENDIAN<int32_t>(0x02a902d6)}};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[22]{NBTTags::IntArray,
                            '\x04',
                            '\x00',
                            'I',
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
                            '\x11',
                            '\x11',
                            '\x11'};
    N = 22;
    exp = Expected<IntArray>{{ParseResult::SUCCESS, 4, "IArr"},
                             2,
                             {solis::FROM_BIG_ENDIAN<int32_t>(0x018065e7),
                              solis::FROM_BIG_ENDIAN<int32_t>(0x02a902d6)}};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{
        NBTTags::IntArray, '\x04', '\x00', 'I', 'A', 'r',
    };
    N = 6;
    exp = Expected<IntArray>{{ParseResult::UNFINISHED, 4, "IAr"}, 0, {}};
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