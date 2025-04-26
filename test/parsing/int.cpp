#include <cstring>
#include <doctest.h>
#include <minecraft/nbt/io/parser_primitives.hpp>

using namespace minecraft::nbt;

TEST_CASE("nbt::Int parsing") {
  // Input
  auto parser = NBTIntParser();
  uint8_t *bytes = nullptr;
  size_t N;

  // Expected
  struct Expected {
    ParseResult result;
    size_t name_size;
    const char *name;
    int32_t value;
  };
  Expected exp;

  // Define subcases
  SUBCASE("Expected") {
    bytes = new uint8_t[10]{NBTTags::Int,
                            '\x03',
                            '\x00',
                            'I',
                            'n',
                            't',
                            static_cast<unsigned char>('\x80'),
                            static_cast<unsigned char>('\x01'),
                            static_cast<unsigned char>('\xa5'),
                            static_cast<unsigned char>('\x08')};
    N = 10;
    exp = Expected{ParseResult::SUCCESS, 3, "Int",
                   solis::FROM_BIG_ENDIAN<int32_t>(0x8001a508)};
  }
  SUBCASE("Too much") {
    bytes = new uint8_t[14]{NBTTags::Int,
                            '\x05',
                            '\x00',
                            'H',
                            'e',
                            'l',
                            'l',
                            'o',
                            '\x12',
                            '\x08',
                            (uint8_t)'\xe7',
                            (uint8_t)'\x01',
                            '\x02',
                            '\x03'};
    N = 14;
    exp = Expected{ParseResult::SUCCESS, 5, "Hello",
                   solis::FROM_BIG_ENDIAN<int32_t>(0x1208e701)};
  }
  SUBCASE("Not enough") {
    bytes = new uint8_t[6]{NBTTags::Int, '\x05', '\x00', 'T', 'e', 's'};
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
  delete b;
}