#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_primitives.hpp>
#include <minecraft/nbt/tests/tests.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::Byte parsing") {
  // Input
  auto parser = NBTByteParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  // struct Expected {
  //   ParseResult result;
  //   size_t name_size;
  //   const char *name;
  //   int8_t value;
  // };
  Expected<Byte> exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[8]{
        NBTTags::Byte, '\x04', '\x00', 'B',
        'y',           't',    'e',    static_cast<unsigned char>('\x80')};
    N = 8;
    exp = Expected<Byte>{ParseResult::SUCCESS, 4, "Byte", '\x80'};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[11]{NBTTags::Byte, '\x05', '\x00', 'H',    'e',   'l',
                            'l',           'o',    '\x12', '\x02', '\x03'};
    N = 11;
    exp = Expected<Byte>{ParseResult::SUCCESS, 5, "Hello", 0x12};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{NBTTags::Byte, '\x05', '\x00', 'T', 'e', 's'};
    N = 6;
    exp = Expected<Byte>{ParseResult::UNFINISHED, 5, "Tes", 0x00};
  }

  // Test the case
  uint8_t *p = bytes;
  auto ret = parser.parse(&p, N);
  auto b = parser.getParsed();

  exp.compare_to(ret, b);

  CHECK(ret == exp.result);
  CHECK(b->getNameSize() == exp.name_size);
  CHECK(strncmp(b->getName(), exp.name, b->getNameSize()) == 0);
  CHECK(b->val == exp.value);

  if (bytes != nullptr)
    delete bytes;
  delete b;
}