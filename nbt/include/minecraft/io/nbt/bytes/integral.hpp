// ============================================================================
// Project: SOLISMC-FILEIO
//
// Integral-types NBT parser & writer declaration
//
// Author    Meltwin (github@meltwin.fr)
// Date      04/06/2026 (created 04/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_BYTE_PARSING_INTEGRAL
#define SOLISMC_IO_NBT_BYTE_PARSING_INTEGRAL

#include "minecraft/io/nbt/bytes/interface.hpp"
#include <concepts>
#include <cstdint>

namespace minecraft::nbt::byte::base {

// ============================================================================

/**
 * @brief Implementation of the byte parser for the integral types (int, short,
 * ...)
 */
template <std::integral T, GameVersion GV>
struct IntegralByteParser : public ByteParserInterface {

  static constexpr uint8_t TYPE_LENGTH = sizeof(T);

  ParseResult parse(Stream &strm, Size &n) override;

  inline bool is_done() const override { return read_bytes == TYPE_LENGTH; }

  void reset() override;

  /**
   * @brief Get the read value if the parser is done, else 0
   */
  inline T get() const { return (is_done()) ? value : 0; }

  inline FieldValue get_value() const override { return FieldValue(get()); }

private:
  T value{0};
  uint8_t read_bytes{0};
};

// ============================================================================

/**
 * @brief Implementation of the byte writer for the integral types (int, short,
 * ...)
 */
template <std::integral T, GameVersion GV>
struct IntegralByteWriter : public ByteWriterInterface {

  static constexpr uint8_t TYPE_LENGTH = sizeof(T);

  /**
   * @brief Set the value to write
   */
  inline void set(const T in_value) {
    reset();
    value = in_value;
  }

  inline bool is_done() const override { return written_bytes == TYPE_LENGTH; }

  WriteResult write(Buffer &strm, Size &n) override;

  void reset() override;

private:
  T value{0};
  uint8_t written_bytes{0};
};

// ============================================================================
// Register common parser implementation
// ============================================================================
template <std::integral T>
REGISTER_TEMPLATED_BYTE_PARSER(T, GameVersion::JAVA, IntegralByteParser);
template <std::integral T>
REGISTER_TEMPLATED_BYTE_PARSER(T, GameVersion::BEDROCK, IntegralByteParser);
template <std::integral T>
REGISTER_TEMPLATED_BYTE_WRITER(T, GameVersion::JAVA, IntegralByteWriter)
template <std::integral T>
REGISTER_TEMPLATED_BYTE_WRITER(T, GameVersion::BEDROCK, IntegralByteWriter)

DECLARE_COMMON_NBT_PARSER(GameVersion::JAVA, int8_t, int16_t, int32_t, int64_t);
DECLARE_COMMON_NBT_PARSER(GameVersion::JAVA, uint8_t, uint16_t, uint32_t,
                          uint64_t);
DECLARE_COMMON_NBT_PARSER(GameVersion::BEDROCK, int8_t, int16_t, int32_t,
                          int64_t);
DECLARE_COMMON_NBT_PARSER(GameVersion::BEDROCK, uint8_t, uint16_t, uint32_t,
                          uint64_t);
DECLARE_COMMON_NBT_WRITER(GameVersion::JAVA, int8_t, int16_t, int32_t, int64_t);
DECLARE_COMMON_NBT_WRITER(GameVersion::JAVA, uint8_t, uint16_t, uint32_t,
                          uint64_t);
DECLARE_COMMON_NBT_WRITER(GameVersion::BEDROCK, int8_t, int16_t, int32_t,
                          int64_t);
DECLARE_COMMON_NBT_WRITER(GameVersion::BEDROCK, uint8_t, uint16_t, uint32_t,
                          uint64_t);

} // namespace minecraft::nbt::byte::base

#endif