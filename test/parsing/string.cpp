#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_primitives.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::String parsing") {
  // Input
  auto parser = NBTStringParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  struct Expected {
    ParseResult result;
    size_t name_size;
    size_t value_size;
    const char *name;
    const char *value;
  };
  Expected exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[22]{"\x08\x06\x00String\x08\x00HelloWorld"};
    N = 21;
    exp = Expected{ParseResult::SUCCESS, 6, 8, "String", "HelloWorld"};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[25]{"\x08\x06\x00String\x0a\x00HelloWorld\x08\x06\x00"};
    N = 25;
    exp = Expected{ParseResult::SUCCESS, 6, 10, "String", "HelloWorld"};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[7]{"\x08\x06\x00Str"};
    N = 6;
    exp = Expected{ParseResult::UNFINISHED, 6, 0, "Str", ""};
  }

  // Test the case
  uint8_t *p = bytes;
  auto ret = parser.parse(&p, N);
  auto b = parser.getParsed();

  CHECK(ret == exp.result);
  CHECK(b->getNameSize() == exp.name_size);
  CHECK(strncmp(b->getName(), exp.name, b->getNameSize()) == 0);
  CHECK(b->val_size == exp.value_size);
  CHECK(strncmp(b->val, exp.value, b->val_size) == 0);

  if (bytes != nullptr)
    delete bytes;
  delete b;
}