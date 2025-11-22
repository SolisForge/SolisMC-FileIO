// ============================================================================
// Project: SOLISMC_FILEIO
//
// Definition of NBT integral parser (short, int, long, ...)
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_BYTE_PARSING_INTEGRAL_HPP
#define SOLISMC_NBT_BYTE_PARSING_INTEGRAL_HPP

#include "minecraft/nbt/parser/base.hpp"
#include "minecraft/nbt/types/base.hpp"
#include "minecraft/nbt/types/integral.hpp"
#include <cstdint>
#include <iostream>

namespace minecraft::nbt {

/**
 * @brief Parser implementation for integral types
 *
 * @tparam T the STD parsed integral type
 */
template <typename T> struct IntegralParser : _IParser {

  ParseResult::E parse(char **strm, unsigned long &N) override {
    NBT_PARSE_N_BYTE_BEGIN()
#if _CMAKE_ENDIANNESS == 1
    value_ += ((T)(*strm)[0]);
    if (n_bytes < TYPE_SIZE - 1)
      value_ = value_ << BIT_PER_BYTE;
#else
    value_ += (T)((T)(*strm)[0]) << n_bytes * BIT_PER_BYTES;
#endif
    NBT_PARSE_N_BYTE_END(n_bytes, TYPE_SIZE);
    return ParseResult::SUCCESS;
  }

  /**
   * @brief Return the value parsed from the stream
   */
  inline T get() const { return value_; }

  inline void reset() override {
    value_ = 0;
    n_bytes = 0;
  }

protected:
  T value_;
  size_t n_bytes = 0;
  const size_t TYPE_SIZE = sizeof(T);
};

// Define parser aliases
MK_BYTE_PARSER_WRAPPER(Tags::Byte, IntegralParser<int8_t>)
MK_BYTE_PARSER_WRAPPER(Tags::Short, IntegralParser<int16_t>)
MK_BYTE_PARSER_WRAPPER(Tags::Int, IntegralParser<int32_t>)
MK_BYTE_PARSER_WRAPPER(Tags::Long, IntegralParser<int64_t>)

} // namespace minecraft::nbt

#endif