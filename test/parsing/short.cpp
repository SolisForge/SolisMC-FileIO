#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_primitives.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::Short parsing") {
  // Input
  auto parser = NBTShortParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  struct Expected {
    ParseResult result;
    size_t name_size;
    const char *name;
    int16_t value;
  };
  Expected exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[10]{
        NBTTags::Tags::Short, '\x05',         '\x00', 'S', 'h', 'o', 'r', 't',
        (uint8_t)'\x80',      (uint8_t)'\x01'};
    N = 10;
    exp = Expected{ParseResult::SUCCESS, 5, "Short",
                   solis::FROM_BIG_ENDIAN<int16_t>(0x8001)};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[12]{NBTTags::Tags::Short,
                            '\x05',
                            '\x00',
                            'H',
                            'e',
                            'l',
                            'l',
                            'o',
                            (uint8_t)'\x12',
                            (uint8_t)'\x85',
                            '\x02',
                            '\x03'};
    N = 12;
    exp = Expected{ParseResult::SUCCESS, 5, "Hello",
                   solis::FROM_BIG_ENDIAN<int16_t>(0x1285)};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{NBTTags::Tags::Short, '\x05', '\x00', 'T', 'e', 's'};
    N = 6;
    exp = Expected{ParseResult::UNFINISHED, 5, "Tes", 0x00};
  }

  // Test the case
  uint8_t *p = bytes;
  auto ret = parser.parse(&p, N);
  auto b = parser.getParsed();

  CHECK(ret == exp.result);
  CHECK(b->getNameSize() == exp.name_size);
  CHECK(strncmp(b->getName(), exp.name, b->getNameSize()) == 0);
  CHECK(b->val == exp.value);

  if (bytes != nullptr)
    delete bytes;
}