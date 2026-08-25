// ============================================================================
// Project: SOLISMC-FILEIO
//
// Compound byte parser implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      03/08/2026 (created 03/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes.hpp"
#include "minecraft/io/nbt/bytes/struct/compound_parser.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace minecraft::nbt::byte::base {

template <GameVersion GV>
ParseResult get_parser(const Tag tag,
                       std::unique_ptr<ByteParserInterface> &parser) {
#define MK_CASE(type, tag)                                                     \
  case Tag::tag:                                                               \
    parser = std::make_unique<ByteParser<type, GV>>();                         \
    return ParseResult::ENDED;

  switch (tag) {
  case Tag::END:
    parser = std::nullptr_t{};
    return ParseResult::ENDED;
  case Tag::LIST:
    parser = std::nullptr_t{};
    return ParseResult::ENDED;

    // Other cases
    MK_CASE(int8_t, BYTE);
    MK_CASE(int16_t, SHORT);
    MK_CASE(int32_t, INT);
    MK_CASE(int64_t, LONG);
    MK_CASE(float, FLOAT);
    MK_CASE(double, DOUBLE);
    MK_CASE(std::string, STRING);
    MK_CASE(std::vector<int8_t>, BYTE_ARRAY);
    MK_CASE(std::vector<int32_t>, INT_ARRAY);
    MK_CASE(std::vector<int64_t>, LONG_ARRAY);
    MK_CASE(Compound, COMPOUND);
  }
  return ParseResult::WRONG_TYPE;
#undef MK_CASE
}

template <> ParseResult CompoundParser<GameVersion::JAVA>::get_value_parser() {
  return get_parser<GameVersion::JAVA>(this->info_.tag, this->value_parser_);
}

template <>
ParseResult CompoundParser<GameVersion::BEDROCK>::get_value_parser() {
  return get_parser<GameVersion::BEDROCK>(this->info_.tag, this->value_parser_);
}

} // namespace minecraft::nbt::byte::base