#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_primitives.hpp>
#include <minecraft/nbt/tests/tests.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::String parsing") {
  // Input
  auto parser = NBTStringParser();
  uint8_t *bytes = nullptr;
  size_t N;
  Expected<String> exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[22]{"\x08\x06\x00String\x08\x00HelloWorld"};
    N = 21;
    exp = Expected<String>{ParseResult::SUCCESS, 6, "String", 8, "HelloWorld"};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[25]{"\x08\x06\x00String\x0a\x00HelloWorld\x08\x06\x00"};
    N = 25;
    exp = Expected<String>{ParseResult::SUCCESS, 6, "String", 10, "HelloWorld"};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[7]{"\x08\x06\x00Str"};
    N = 6;
    exp = Expected<String>{ParseResult::UNFINISHED, 6, "Str", 0, ""};
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