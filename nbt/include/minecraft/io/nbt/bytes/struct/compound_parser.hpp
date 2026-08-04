// ============================================================================
// Project: SOLISMC-FILEIO
//
// Generic compound object import logic
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_BYTE_PARSING_COMPOUND_PARSER
#define SOLISMC_NBT_BYTE_PARSING_COMPOUND_PARSER
#include "minecraft/io/nbt/bytes/interface.hpp"
#include "minecraft/io/nbt/bytes/struct/object_parser_interface.hpp"
#include "minecraft/io/nbt/compound.hpp"
#include "minecraft/io/nbt/field.hpp"
#include "minecraft/io/nbt/tag.hpp"
#include <bitset>
#include <cstddef>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace minecraft::nbt::byte::base {

// ============================================================================

/**
 * @brief Implementation of the byte parser for C++ objects.
 *
 * @tparam Adapter adapter to fill the C++ structure
 * @tparam GV the GameVersion to use
 */
template <GameVersion GV> struct CompoundParser : ObjectParserInterface<GV> {

  /**
   * @brief Construct a new parser with a new internally initialized object
   */
  explicit CompoundParser<GV>() : ObjectParserInterface<GV>() {}

  inline Compound &&get() { return std::move(obj); }

  FieldValue get_value() const noexcept override {
    return FieldValue(Tag::COMPOUND, obj);
  }

protected:
  Compound obj;

  ParseResult get_value_parser() override;

  void set_value() override {
    obj.set(this->info_.name, this->value_parser_->get_value());
  }
};

template <> REGISTER_BYTE_PARSER(Compound, GameVersion::JAVA, CompoundParser);
template <>
REGISTER_BYTE_PARSER(Compound, GameVersion::BEDROCK, CompoundParser);

DECLARE_COMMON_NBT_PARSER_IMPL(GameVersion::JAVA, Compound);
DECLARE_COMMON_NBT_PARSER_IMPL(GameVersion::BEDROCK, Compound);

} // namespace minecraft::nbt::byte::base
#endif