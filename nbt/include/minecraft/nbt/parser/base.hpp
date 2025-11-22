// ============================================================================
// Project: SOLISMC_FILEIO
//
// Definition of the bytes -> NBT parser
//
// Author    Meltwin (github@meltwin.fr)
// Date      19/11/2025 (created 19/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_PARSER_HPP
#define SOLISMC_NBT_PARSER_HPP

#include "minecraft/nbt/types/base.hpp"
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
struct ParseResult {
  enum E : uint8_t { SUCCESS, UNFINISHED, FAILED };

  // --------------------------------------------------------------------------

  /**
   * @brief Get the name of the given result as a const char*
   */
  static constexpr const char *getName(ParseResult::E tag) {
    // Macro to easily generate all the conversions
#define MK_CASE(name)                                                          \
  case ParseResult::E::name:                                                   \
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
};

// ============================================================================
/**
 * @brief NBT bytes -> parser interface defining all common methods
 */
struct _IParser {

  /**
   * @brief Parse the given buffer to set the value to the NBT type
   *
   * @param buffer the buffer to read the value from
   * @param N the number of bytes left in the buffer
   */
  virtual ParseResult::E parse(char **strm, unsigned long &N) = 0;

  /**
   * @brief Reset the parser for a new usage
   */
  virtual void reset() = 0;

  static void inc_stream(char **strm, unsigned long &N, unsigned int inc = 1) {
    (*strm) += inc;
    N -= inc;
  }
};

// ============================================================================
// Parser common entrypoing
// ============================================================================

/**
 * @brief Alias to instantiate any byte -> NBT parser implementation
 *
 * @tparam TAG
 */
template <Tags::E TAG> struct BytesParser : _IParser {};

/**
 * @brief Macro to generate a BytesParser specialization for a tag, given a
 * parser implementation.
 *
 * This class helps gathering all parser implementation below a common name.
 */
#define MK_BYTE_PARSER_WRAPPER(tag, impl)                                      \
  template <> struct BytesParser<tag> : impl {                                 \
    static_assert(std::is_base_of_v<_IParser, impl>,                           \
                  "The NBT parsing implementation " #impl                      \
                  " must be a child of _IParser");                             \
  };

// ============================================================================
// Parsing utility functions
// ============================================================================
constexpr uint8_t BIT_PER_BYTES{8};

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