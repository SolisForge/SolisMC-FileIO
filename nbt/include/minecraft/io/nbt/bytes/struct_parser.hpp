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
#include "minecraft/io/nbt/bytes/string.hpp"
#include "minecraft/io/nbt/bytes/struct_adapter.hpp"
#include "minecraft/io/nbt/tag.hpp"
#include <bitset>
#include <cstddef>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace minecraft::nbt::byte::base {

// ============================================================================
// Struct parser
// ============================================================================

enum class ParserState : uint8_t { TAG, NAME, COMPLETE };

// ============================================================================

/**
 * @brief Implementation of the byte parser for C++ objects.
 *
 * @tparam Adapter adapter to fill the C++ structure
 * @tparam GV the GameVersion to use
 */
template <typename C, WriterAdapterImplementation Adapter, GameVersion GV>
struct ObjectParser : ByteParserInterface {

  /**
   * @brief Construct a new parser with a new internally initialized object
   */
  explicit ObjectParser<C, Adapter, GV>()
      : ByteParserInterface(), object_(std::make_shared<C>()),
        adapter_(Adapter(object_)) {}

  /**
   * @brief Construct a new parser with a provided initialized object
   */
  ObjectParser<C, Adapter, GV>(std::shared_ptr<C> obj)
      : ByteParserInterface(), object_(obj), adapter_(Adapter(object_)) {}

  /**
   * @brief Parse the object from the given byte stream
   *
   * @param strm the byte stream to read from
   * @param n the number of bytes left in the stream
   * @return the state of the parsing
   */
  ParseResult parse(Stream &strm, Size &n) override {
    // Reset parser if previous object parsing is done
    if (is_done())
      reset();

    // Parse each field
    while (!is_done()) {
      if (auto ret = parse_field(strm, n); ret != ParseResult::ENDED)
        return ret;
      if (info_.tag == Tag::END) {
        state_.set((uint8_t)ParserState::COMPLETE);
        break;
      }

      // Reset state
      state_.set((uint8_t)ParserState::TAG, false);
      state_.set((uint8_t)ParserState::NAME, false);
      name_parser_.reset();
      value_parser_ = std::nullptr_t{};
    }
    // Mark as done
    state_.set((uint8_t)ParserState::COMPLETE);
    return ParseResult::ENDED;
  }

  /**
   * @brief Has the parser finished reading the last value
   */
  bool is_done() const override {
    return state_.test((uint8_t)ParserState::COMPLETE);
  };

  /**
   * @brief Reset the internal state of the parser
   */
  void reset() override { state_.reset(); };

  std::shared_ptr<C> get() const { return object_; }
  FieldValue get_value() const override { return FieldValue(get()); }

protected:
  std::shared_ptr<C> object_;
  Adapter adapter_;
  StringByteParser<GV> name_parser_;
  std::unique_ptr<ByteParserInterface> value_parser_ = std::nullptr_t{};
  FieldInfo info_;
  std::bitset<3> state_;

  /**
   * @brief Parse a single field from the stream
   *
   * @param strm the byte stream to read from
   * @param n the number of bytes left in the stream
   */
  ParseResult parse_field(Stream &strm, Size &n) {
    // Not enough bytes to end the parsing
    if (n == 0)
      return ParseResult::UNFINISHED;

    // Parse type tag information
    if (!state_.test((uint8_t)ParserState::TAG)) {
      info_.tag = from_int(strm[0]);
      strm++;
      n--;
      state_.set((uint8_t)ParserState::TAG);

      // If end of object
      if (info_.tag == Tag::END)
        return ParseResult::ENDED;
    }

    // Parse field name
    if (!state_.test((uint8_t)ParserState::NAME)) {
      // Reset state
      if (auto ret = name_parser_.parse(strm, n); ret != ParseResult::ENDED)
        return ret;
      info_.name = std::move(name_parser_.get());
      state_.set((uint8_t)ParserState::NAME);

      // Get value parser by this field
      switch (adapter_.is_field(info_)) {
      case FieldState::WRONG_TYPE:
        return ParseResult::WRONG_TYPE;
      case FieldState::DO_NOT_EXIST:
        return ParseResult::UNKNOWN_FIELD;
      default:
        break;
      }
      value_parser_ = adapter_.get_byte_parser(info_);

      // TODO: use default parser for tag instead
    }

    // Parse field value
    if (auto ret = value_parser_->parse(strm, n); ret != ParseResult::ENDED)
      return ret;
    adapter_.set_field(info_, value_parser_->get_value());

    return ParseResult::ENDED;
  }
};

} // namespace minecraft::nbt::byte::base
#endif