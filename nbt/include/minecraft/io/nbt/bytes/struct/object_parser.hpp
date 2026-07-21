// ============================================================================
// Project: SOLISMC-FILEIO
//
// C++ struct import/export logic
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_BYTE_PARSING_STRUCT_PARSER
#define SOLISMC_NBT_BYTE_PARSING_STRUCT_PARSER
#include "minecraft/io/nbt/bytes/field.hpp"
#include "minecraft/io/nbt/bytes/interface.hpp"
#include "minecraft/io/nbt/bytes/struct/adapter.hpp"
#include "minecraft/io/nbt/bytes/struct/object_parser_interface.hpp"
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
template <typename C, WriterAdapterImplementation Adapter, GameVersion GV>
struct ObjectParser : ObjectParserInterface<GV> {

  /**
   * @brief Construct a new parser with a new internally initialized object
   */
  explicit ObjectParser<C, Adapter, GV>()
      : ObjectParserInterface<GV>(), object_(std::make_shared<C>()),
        adapter_(Adapter(object_)) {}

  /**
   * @brief Construct a new parser with a provided initialized object
   */
  explicit ObjectParser<C, Adapter, GV>(std::shared_ptr<C> obj)
      : ObjectParserInterface<GV>(), object_(obj), adapter_(Adapter(object_)) {}

  std::shared_ptr<C> get() const noexcept { return object_; }
  FieldValue get_value() const noexcept override { return FieldValue(get()); }

protected:
  std::shared_ptr<C> object_;
  Adapter adapter_;

  ParseResult get_value_parser() override {
    // Get value parser by this field
    switch (adapter_.is_field(this->info_)) {
    case FieldState::WRONG_TYPE:
      return ParseResult::WRONG_TYPE;
    case FieldState::DO_NOT_EXIST:
      return ParseResult::UNKNOWN_FIELD;
    default:
      break;
    }
    this->value_parser_ = adapter_.get_byte_parser(this->info_);
    return ParseResult::ENDED;
  }

  void set_value() override {
    adapter_.set_field(this->info_, this->value_parser_->get_value());
  }
};

} // namespace minecraft::nbt::byte::base
#endif