#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <iostream>
#include <minecraft/nbt/io/parser_primitives.hpp>

using namespace minecraft::nbt;

TEST_CASE("byte") {
  // Input
  // auto parser = NBTParser::make<Byte, NBTTags::Byte>();
  auto parser = NBTByteParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  struct Expected {
    ParseResult result;
    size_t name_size;
    const char *name;
    int8_t value;
  };
  Expected exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[8]{
        '\x01', '\x00', '\x04', 'B',
        'y',    't',    'e',    static_cast<unsigned char>('\x80')};
    N = 8;
    exp = Expected{ParseResult::SUCCESS, 4, "Byte", '\x80'};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[11]{'\x01', '\x00', '\x05', 'H',   'e',  'l',
                            'l',    'o',    '\x12', '\02', '\03'};
    N = 11;
    exp = Expected{ParseResult::SUCCESS, 5, "Hello", 0x12};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{'\x01', '\x00', '\x05', 'T', 'e', 's'};
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