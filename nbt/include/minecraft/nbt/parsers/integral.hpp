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
#ifndef SOLISMC_NBT_PARSER_INTEGRAL_HPP
#define SOLISMC_NBT_PARSER_INTEGRAL_HPP

#include "minecraft/nbt/parsers/base.hpp"
#include <iostream>

namespace minecraft::nbt {

// ============================================================================

/**
 * @brief Parser implementation for integral types
 *
 * @tparam T the STD parsed integral type
 */
template <typename T> struct IntegralParser : _IParser {

  ParseResult parse(const StreamChar *&strm, unsigned long &N) override {
    NBT_PARSE_N_BYTE_BEGIN()
    std::cout << "N_bytes=" << n_bytes << " " << value_ << " + "
              << static_cast<T>(strm[0]) << " = ";
#if _CMAKE_BIG_ENDIAN == 0
    value_ += ((T)strm[0]);
    if (n_bytes < TYPE_SIZE - 1)
      value_ = static_cast<T>(value_ << BIT_PER_BYTES);
#else
    value_ += static_cast<T>(strm[0]) << n_bytes * BIT_PER_BYTES;
#endif
    std::cout << value_ << std::endl;
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

private:
  T value_;
  size_t n_bytes = 0;
  const size_t TYPE_SIZE = sizeof(T);
};

// Define parser aliases
// ============================================================================
MK_BYTE_PARSER_WRAPPER(Tags::Byte, IntegralParser<int8_t>)
MK_BYTE_PARSER_WRAPPER(Tags::Short, IntegralParser<int16_t>)
MK_BYTE_PARSER_WRAPPER(Tags::Int, IntegralParser<int32_t>)
MK_BYTE_PARSER_WRAPPER(Tags::Long, IntegralParser<int64_t>)

} // namespace minecraft::nbt

#endif