// ============================================================================
// Project: SOLISMC-FILEIO
//
// Base interface for buffered byte-operations classes
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/06/2026 (created 13/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_COMMON_INTERFACE
#define SOLISMC_IO_NBT_COMMON_INTERFACE

#include "minecraft/io/nbt/bytes/field.hpp"
#include <bit>
#include <cstdint>
#include <solis/utils/macros.hpp>
#include <type_traits>

namespace minecraft::nbt {

// ============================================================================
// Common types
// ============================================================================
using Stream = const char *;
using Buffer = char *;
using Size = std::size_t;
constexpr auto BIT_PER_BYTE{8};

enum class ParseResult : uint8_t {
  ENDED,
  UNFINISHED,
  UNKNOWN_FIELD,
  WRONG_TYPE,
};
enum class WriteResult : uint8_t { ENDED, UNFINISHED };

enum class GameVersion { JAVA, BEDROCK };

// Endianness
template <GameVersion GV> struct NBTEndianness;
template <> struct NBTEndianness<GameVersion::JAVA> {
  static constexpr auto value = std::endian::big;
};
template <> struct NBTEndianness<GameVersion::BEDROCK> {
  static constexpr auto value = std::endian::little;
};

namespace byte::base {
// ============================================================================
// Interfaces
// ============================================================================

/**
 * @brief Interface for the NBT byte parser implementations
 */
struct ByteParserInterface {

  virtual ~ByteParserInterface() = default;

  /**
   * @brief Parse the object from the given byte stream
   *
   * @param strm the byte stream to read from
   * @param n the number of bytes left in the stream
   * @return the state of the parsing
   */
  virtual ParseResult parse(Stream &strm, Size &n) = 0;

  /**
   * @brief Get the value contained inside a FieldValue object
   */
  virtual FieldValue get_value() const = 0;

  /**
   * @brief Has the parser finished reading the last value
   */
  virtual bool is_done() const = 0;

  /**
   * @brief Reset the internal state of the parser
   */
  virtual void reset() = 0;
};

/**
 * @brief General byte parser
 *
 * @tparam typename
 * @tparam GameVersion
 */
template <typename, GameVersion> struct ByteParser;

// ============================================================================

/**
 * @brief Interface for the NBT byte writer implementations
 */
struct ByteWriterInterface {

  virtual ~ByteWriterInterface() = default;

  /**
   * @brief Write the object in the given byte stream
   *
   * @param strm the byte stream to write to
   * @param n the number of bytes available for writing in the stream
   * @return the state of the writing operation
   */
  virtual WriteResult write(Buffer &strm, Size &n) = 0;

  /**
   * @brief Has the parser finished writing the last value
   */
  virtual bool is_done() const = 0;

  /**
   * @brief Reset the internal state of the parser
   */
  virtual void reset() = 0;
};

/**
 * @brief General byte writer
 *
 * @tparam typename
 * @tparam GameVersion
 */
template <typename, GameVersion> struct ByteWriter;

// ============================================================================
// Concepts
// ============================================================================

template <class T>
concept IsParserImplementation = std::is_base_of_v<ByteParserInterface, T>;
template <class T>
concept IsWriterImplementation = std::is_base_of_v<ByteWriterInterface, T>;

// ============================================================================
// Parser & writer validator
// ============================================================================

/**
 * @brief Validator for NBT byte parser.
 *
 * @tparam _Impl implementation to use to parse this object
 */
template <IsParserImplementation _Impl>
struct ByteParserValidator : public _Impl {};

/**
 * @brief Validator for NBT byte writer.
 */
template <IsWriterImplementation _Impl>
struct ByteWriterValidator : public _Impl {};

} // namespace byte::base

// ============================================================================
// Helper macros
// ============================================================================
/**
 * @brief Register the byte parser implementation
 */
#define REGISTER_TEMPLATED_BYTE_PARSER(MetaVar, GameVersion, Impl)             \
  struct ByteParser<MetaVar, GameVersion>                                      \
      : ByteParserValidator<Impl<MetaVar, GameVersion>> {};

/**
 * @brief Register the byte parser implementation
 */
#define REGISTER_BYTE_PARSER(MetaVar, GameVersion, Impl)                       \
  struct ByteParser<MetaVar, GameVersion>                                      \
      : ByteParserValidator<Impl<GameVersion>> {};

/**
 * @brief Register the byte writer implementation
 */
#define REGISTER_TEMPLATED_BYTE_WRITER(MetaVar, GameVersion, Impl)             \
  struct ByteWriter<MetaVar, GameVersion>                                      \
      : ByteWriterValidator<Impl<MetaVar, GameVersion>> {};
/**
 * @brief Register the byte writer implementation
 */
#define REGISTER_BYTE_WRITER(MetaVar, GameVersion, Impl)                       \
  struct ByteWriter<MetaVar, GameVersion>                                      \
      : ByteWriterValidator<Impl<GameVersion>> {};

// ================================ IN HEADER =================================
#define DECLARE_EXPORTED_TEMPLATE(Template) extern template struct Template;

#define DECLARE_COMMON_NBT_PARSER_IMPL(GV, T)                                  \
  extern template struct ByteParser<T, GV>;

/**
 * @brief Register a common parser implementation in headers
 */
#define DECLARE_COMMON_NBT_PARSER(...)                                         \
  FOR_EACH2(DECLARE_COMMON_NBT_PARSER_IMPL, __VA_ARGS__)

/**
 * @brief Register a common writer implementation in headers
 */
#define __DECLARE_COMMON_NBT_WRITER__IMPL(GV, T)                               \
  extern template struct ByteWriter<T, GV>;

#define DECLARE_COMMON_NBT_WRITER(...)                                         \
  FOR_EACH2(__DECLARE_COMMON_NBT_WRITER__IMPL, __VA_ARGS__)

// ================================ IN SOURCE =================================
#define EXPORT_TEMPLATE(Template) template struct Template;

/**
 * @brief Export a common parser implementation in source
 */
#define __EXPORT_COMMON_NBT_PARSER__IMPL(GV, T)                                \
  template struct ByteParser<T, GV>;

#define EXPORT_COMMON_NBT_PARSER(...)                                          \
  FOR_EACH2(__EXPORT_COMMON_NBT_PARSER__IMPL, __VA_ARGS__)

/**
 * @brief Export a common writer implementation in source
 */
#define __EXPORT_COMMON_NBT_WRITER__IMPL(GV, T)                                \
  template struct ByteWriter<T, GV>;

#define EXPORT_COMMON_NBT_WRITER(...)                                          \
  FOR_EACH2(__EXPORT_COMMON_NBT_WRITER__IMPL, __VA_ARGS__)

} // namespace minecraft::nbt

#endif