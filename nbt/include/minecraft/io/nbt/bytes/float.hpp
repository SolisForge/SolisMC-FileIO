// ============================================================================
// Project: SOLISMC-FILEIO
//
// Floating-types NBT parser & writer declaration
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/06/2026 (created 13/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_BYTE_PARSING_FLOATING
#define SOLISMC_IO_NBT_BYTE_PARSING_FLOATING

#include "minecraft/io/nbt/bytes/integral.hpp" // IWYU pragma: keep
#include "minecraft/io/nbt/bytes/interface.hpp"
#include <concepts>
#include <cstdint>

namespace minecraft::nbt::byte::base {

template <std::floating_point T> struct IntBuffer;
template <> struct IntBuffer<float> { using T = int32_t; };
template <> struct IntBuffer<double> { using T = int64_t; };

// ============================================================================

/**
 * @brief Implementation of the byte parser for the integral types (int, short,
 * ...)
 */
template <std::floating_point T, GameVersion GV>
struct FloatByteParser : public ByteParserInterface {

  static constexpr uint8_t TYPE_LENGTH = sizeof(T);

  ParseResult parse(Stream &strm, Size &n) override;

  inline bool is_done() const override { return int_parser.is_done(); }

  void reset() override;

  /**
   * @brief Get the read value if the parser is done, else 0
   */
  inline T get() const { return (is_done()) ? value : 0; }

  inline FieldValue get_value() const override { return FieldValue(TypeToTag<T>::TAG, get()); }

private:
  T value{0};
  ByteParser<typename IntBuffer<T>::T, GV> int_parser;
};

// ============================================================================

/**
 * @brief Implementation of the byte writer for the integral types (int, short,
 * ...)
 */
template <std::floating_point T, GameVersion GV>
struct FloatByteWriter : public ByteWriterInterface {};

// ============================================================================
// Register common parser implementation
// ============================================================================
template <std::floating_point T>
REGISTER_TEMPLATED_BYTE_PARSER(T, GameVersion::JAVA, FloatByteParser);
template <std::floating_point T>
REGISTER_TEMPLATED_BYTE_PARSER(T, GameVersion::BEDROCK, FloatByteParser);

DECLARE_COMMON_NBT_PARSER(GameVersion::JAVA, float, double);
DECLARE_COMMON_NBT_PARSER(GameVersion::BEDROCK, float, double);

} // namespace minecraft::nbt::byte::base

#endif