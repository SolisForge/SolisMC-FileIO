// ============================================================================
// Project: SOLISMC-FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      11/12/2025 (created 11/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_PARSER_BASE_HPP
#define SOLISMC_NBT_PARSER_BASE_HPP

#include <cstdint>

namespace minecraft::nbt {

// ============================================================================
// Parser interface
// ============================================================================

/**
 * @brief Possible results of parsing:
 *  - SUCCESS when parsing successfully finished
 *  - UNFINISHED when the parser is missing some bytes
 *  - FAILED when something went wrong and the parser can't finish
 */
enum class ParseResult : uint8_t { SUCCESS, UNFINISHED, FAILED };

// ============================================================================
/**
 * @brief Get the name of the given result as a const char*
 */
[[maybe_unused]] static constexpr const char *getName(ParseResult tag) {
  // Macro to easily generate all the conversions
#define MK_CASE(name)                                                          \
  case ParseResult::name:                                                      \
    return #name;

  // Construct
  switch (tag) {
    MK_CASE(SUCCESS)
    MK_CASE(FAILED)
    MK_CASE(UNFINISHED)
  default:
    return "UNKOWN";
  }

#undef MK_CASE
}

// ============================================================================
using StreamChar = uint8_t;
constexpr uint8_t BIT_PER_BYTES{sizeof(StreamChar) * 8};

// ============================================================================

/**
 * @brief NBT bytes -> parser interface defining all common methods
 */
template <typename T> struct BytesParser {
  static_assert(false, "Not registerd");

  ~BytesParser();

  /**
   * @brief Parse the given buffer to set the value to the NBT type
   *
   * @param buffer the buffer to read the value from
   * @param N the number of bytes left in the buffer
   */
  ParseResult parse(const StreamChar *&strm, unsigned long &N);

  /**
   * @brief Reset the parser for a new usage
   */
  void reset();

  /**
   * @brief Return the value parsed from the stream
   */
  T get() const;
};

/**
 * @brief Increment the given stream by "inc" bytes
 *
 * @param strm the current stream
 * @param N the number of bytes left in the stream
 * @param inc how many bytes we should move forward in the stream
 */
inline void inc_stream(const StreamChar *&strm, unsigned long &N,
                       unsigned int inc = 1) {
  strm += inc;
  N -= inc;
}

// ============================================================================
// Parsing utility functions
// ============================================================================

#define NBT_PARSE_N_BYTE_BEGIN()                                               \
  if (N <= 0)                                                                  \
    return ParseResult::UNFINISHED;                                            \
  do {
#define NBT_PARSE_N_BYTE_END(curr_byte, len)                                   \
  curr_byte++;                                                                 \
  inc_stream(strm, N);                                                         \
  }                                                                            \
  while (N > 0 && curr_byte < len)                                             \
    ;                                                                          \
  if (curr_byte != len) {                                                      \
    return ParseResult::UNFINISHED;                                            \
  }

} // namespace minecraft::nbt

#endif