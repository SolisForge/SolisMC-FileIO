// ============================================================================
// Project: SOLISMC-FILEIO
//
// Common interface for C++ & compound parsers
//
// Author    Meltwin (github@meltwin.fr)
// Date      21/07/2026 (created 21/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_BYTE_PARSING_OBJECT_PARSER_INTERFACE
#define SOLISMC_NBT_BYTE_PARSING_OBJECT_PARSER_INTERFACE

#include "minecraft/io/nbt/bytes/field.hpp"
#include "minecraft/io/nbt/bytes/interface.hpp"
#include "minecraft/io/nbt/bytes/string.hpp"
#include <bitset>

namespace minecraft::nbt::byte::base {

// ============================================================================
// Object parser
// ============================================================================

enum class ParserState : uint8_t { TAG, NAME, COMPLETE };

/**
 * @brief Struct to simplify the use of enumeration & bitset
 */
struct ParserStateBits {

  inline void set(const ParserState v) { state_.set((uint8_t)v, true); }
  inline void reset() { state_.reset(); }
  inline void unset(const ParserState v) { state_.set((uint8_t)v, false); }

  inline bool operator()(const ParserState v) const {
    return state_.test((uint8_t)v);
  }

protected:
  std::bitset<3> state_;
};

// ============================================================================

/**
 * @brief Common interface for a byte parser that translate a NBT (field ->
 * value) map into a loaded object.
 */
template <GameVersion GV> struct ObjectParserInterface : ByteParserInterface {

  explicit ObjectParserInterface<GV>() : ByteParserInterface() {}

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
      if (info_.tag == Tag::END)
        break;

      // Reset state
      state_.unset(ParserState::TAG);
      state_.unset(ParserState::NAME);
      name_parser_.reset();
      value_parser_ = std::nullptr_t{};
    }
    // Mark as done
    state_.set(ParserState::COMPLETE);
    return ParseResult::ENDED;
  }

  /**
   * @brief Has the parser finished reading the last value
   */
  bool is_done() const override { return state_(ParserState::COMPLETE); };

  /**
   * @brief Reset the internal state of the parser
   */
  void reset() override { state_.reset(); };

protected:
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
    if (!state_(ParserState::TAG)) {
      info_.tag = from_int(strm[0]);
      strm++;
      n--;
      state_.set(ParserState::TAG);

      // If end of object
      if (info_.tag == Tag::END)
        return ParseResult::ENDED;
    }

    // Parse field name
    if (!state_(ParserState::NAME)) {
      // Reset state
      if (auto ret = name_parser_.parse(strm, n); ret != ParseResult::ENDED)
        return ret;
      info_.name = std::move(name_parser_.get());
      state_.set(ParserState::NAME);

      // Get the right parser for this field
      if (auto ret = get_value_parser(); ret != ParseResult::ENDED) {
        return ret;
      }
    }

    // Parse field value
    if (auto ret = value_parser_->parse(strm, n); ret != ParseResult::ENDED)
      return ret;
    set_value();

    return ParseResult::ENDED;
  }

  /**
   * @brief Get the right value parser for the parsed field information
   *
   * @return a unique pointer to a byte parser
   */
  virtual ParseResult get_value_parser() = 0;
  /**
   * @brief Set the value into the object from the parsed value
   */
  virtual void set_value() = 0;

protected:
  StringByteParser<GV> name_parser_;
  std::unique_ptr<ByteParserInterface> value_parser_ = std::nullptr_t{};
  FieldInfo info_;
  ParserStateBits state_;
};

} // namespace minecraft::nbt::byte::base

#endif