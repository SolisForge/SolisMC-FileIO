#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_primitives.hpp>
#include <minecraft/nbt/tests/tests.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::Long parsing") {
  // Input
  auto parser = NBTLongParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  Expected<Long> exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[15]{NBTTags::Long,
                            '\x04',
                            '\x00',
                            'L',
                            'o',
                            'n',
                            'g',
                            static_cast<uint8_t>('\x80'),
                            static_cast<uint8_t>('\x01'),
                            static_cast<uint8_t>('\xa5'),
                            static_cast<uint8_t>('\x08'),
                            static_cast<uint8_t>('\x80'),
                            static_cast<uint8_t>('\x01'),
                            static_cast<uint8_t>('\xa5'),
                            static_cast<uint8_t>('\x08')};
    N = 15;
    exp = Expected<Long>{ParseResult::SUCCESS, 4, "Long",
                         solis::FROM_BIG_ENDIAN<int64_t>(0x8001a5088001a508)};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[18]{NBTTags::Long,
                            '\x05',
                            '\x00',
                            'H',
                            'e',
                            'l',
                            'l',
                            'o',
                            static_cast<uint8_t>('\x12'),
                            static_cast<uint8_t>('\x08'),
                            static_cast<uint8_t>('\xe7'),
                            static_cast<uint8_t>('\x01'),
                            static_cast<uint8_t>('\x12'),
                            static_cast<uint8_t>('\x08'),
                            static_cast<uint8_t>('\xe7'),
                            static_cast<uint8_t>('\x01'),
                            '\x02',
                            '\x03'};
    N = 18;
    exp = Expected<Long>{ParseResult::SUCCESS, 5, "Hello",
                         solis::FROM_BIG_ENDIAN<int64_t>(0x1208e7011208e701)};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{NBTTags::Long, '\x05', '\x00', 'T', 'e', 's'};
    N = 6;
    exp = Expected<Long>{ParseResult::UNFINISHED, 5, "Tes", 0x00};
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