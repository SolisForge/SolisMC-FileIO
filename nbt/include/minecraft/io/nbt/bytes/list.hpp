// ============================================================================
// Project: SOLISMC-FILEIO
//
// NBT lists parser & writer declaration
//
// Author    Meltwin (github@meltwin.fr)
// Date      04/06/2026 (created 04/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_BYTE_PARSING_LIST
#define SOLISMC_IO_NBT_BYTE_PARSING_LIST

#include "minecraft/io/nbt/bytes/integral.hpp" // IWYU pragma: keep
#include "minecraft/io/nbt/tag.hpp"
#include <vector>

namespace minecraft::nbt::byte::base {

// ============================================================================

/**
 * @brief Implementation of the byte parser for the integral types (int, short,
 * ...)
 * The full implementation must be made in the header file to allow the parsing
 * of list of any element.
 */
template <typename T, GameVersion GV>
struct ListByteParser : public ByteParserInterface {

  ParseResult parse(Stream &strm, Size &n) override {
    // Reset parser if last parsing has finished to reset state
    if (is_done())
      reset();

    // Parse list size
    if (!size_parser.is_done()) {
      if (auto ret = size_parser.parse(strm, n); ret != ParseResult::ENDED)
        return ret;

      value.resize(size_parser.get());
    }

    // Parse values
    for (size_t i = read_elements; i < size_parser.get(); i++) {
      if (auto ret = value_parser.parse(strm, n); ret != ParseResult::ENDED)
        break;

      // Save value into the vector
      value[i] = value_parser.get();
      value_parser.reset();
      read_elements++;
    }
    return (is_done()) ? ParseResult::ENDED : ParseResult::UNFINISHED;
  }

  bool is_done() const override {
    return size_parser.is_done() && read_elements == size_parser.get();
  }

  void reset() override {
    read_elements = 0;
    value = std::vector<T>{};
    size_parser.reset();
  }

  /**
   * @brief Get the read value if the parser is done, else 0
   */
  inline std::vector<T> get() const {
    return (is_done()) ? std::move(value) : std::vector<T>{};
  }

  inline FieldValue get_value() const override {
    return FieldValue(TypeToTag<std::vector<T>>::TAG, get());
  }

private:
  IntegralByteParser<uint32_t, GV> size_parser;
  ByteParser<T, GV> value_parser;
  std::vector<T> value{};
  uint32_t read_elements{0};
};

// ============================================================================

/**
 * @brief Implementation of the byte writer for the integral types (int, short,
 * ...)
 */
template <typename T, GameVersion GV>
struct ListByteWriter : public ByteWriterInterface {};

// ============================================================================
// Register common parser implementation
// ============================================================================
template <typename T>
struct ByteParser<std::vector<T>, GameVersion::JAVA>
    : ByteParserValidator<ListByteParser<T, GameVersion::JAVA>> {};
template <typename T>
struct ByteParser<std::vector<T>, GameVersion::BEDROCK>
    : ByteParserValidator<ListByteParser<T, GameVersion::BEDROCK>> {};

// ============================================================================
// Register common parser implementation
// ============================================================================
DECLARE_COMMON_NBT_PARSER(GameVersion::JAVA, std::vector<int8_t>,
                          std::vector<int32_t>, std::vector<int64_t>);
DECLARE_COMMON_NBT_PARSER(GameVersion::BEDROCK, std::vector<int8_t>,
                          std::vector<int32_t>, std::vector<int64_t>);

} // namespace minecraft::nbt::byte::base

#endif