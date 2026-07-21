// ============================================================================
// Project: SOLISMC-FILEIO
//
// String-types NBT parser & writer declaration
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/06/2026 (created 13/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_BYTE_PARSING_STRING
#define SOLISMC_IO_NBT_BYTE_PARSING_STRING

#include "minecraft/io/nbt/bytes/integral.hpp" // IWYU pragma: keep
#include "minecraft/io/nbt/bytes/interface.hpp"
#include <cstdint>
#include <string>

namespace minecraft::nbt::byte::base {

/**
 * @brief Implementation of the byte parser for the string types
 */
template <GameVersion GV> struct StringByteParser : public ByteParserInterface {

  ParseResult parse(Stream &strm, Size &n) override;

  bool is_done() const override;

  /**
   * @brief Reset the parser internal state.
   */
  void reset() override;

  /**
   * @brief Get the read value if the parser is done, else an empty string
   */
  inline std::string get() const {
    return (is_done()) ? std::move(value) : std::string{};
  }

  inline FieldValue get_value() const override {
    return FieldValue(Tag::STRING, get());
  }

private:
  /**
   * @brief String size parser.
   * The string size is stored as an 16 bits unsigned integer.
   */
  IntegralByteParser<uint16_t, GV> int_parser;
  std::string value{};
  uint16_t read_bytes{0};
};

// ============================================================================

/**
 * @brief Implementation of the byte writer for the integral types (int, short,
 * ...)
 */
template <GameVersion GV>
struct StringByteWriter : public ByteWriterInterface {};

// ============================================================================
// Register common parser implementation
// ============================================================================
// Register byte parser implementation for strings
template <>
REGISTER_BYTE_PARSER(std::string, GameVersion::JAVA, StringByteParser);
template <>
REGISTER_BYTE_PARSER(std::string, GameVersion::BEDROCK, StringByteParser);

// ============================================================================
// Register common parser implementation
// ============================================================================
// Declare string parser implementations
DECLARE_EXPORTED_TEMPLATE(StringByteParser<GameVersion::JAVA>);
DECLARE_EXPORTED_TEMPLATE(StringByteParser<GameVersion::BEDROCK>);
// Declare byte parser specializations
DECLARE_COMMON_NBT_PARSER(GameVersion::JAVA, std::string);
DECLARE_COMMON_NBT_PARSER(GameVersion::BEDROCK, std::string);

} // namespace minecraft::nbt::byte::base

#endif