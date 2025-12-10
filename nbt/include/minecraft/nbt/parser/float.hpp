// ============================================================================
// Project: SOLISMC_FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      28/11/2025 (created 28/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#ifndef SOLISMC_NBT_BYTE_PARSING_FLOAT_HPP
#define SOLISMC_NBT_BYTE_PARSING_FLOAT_HPP

#include "minecraft/nbt/parser/base.hpp"
#include "minecraft/nbt/parser/integral.hpp"
#include <bit>
#include <cstdint>
#include <type_traits>

namespace minecraft::nbt {

/**
 * @brief Parser implementation for floating point typoes
 *
 * @tparam T the parsed STD type
 */
template <typename T, typename BufferType>
struct FloatingPointParser : _IParser {
  static_assert(std::is_integral_v<BufferType>,
                "The buffer should be of integral type");
  static_assert(sizeof(T) == sizeof(BufferType),
                "The buffer type should be of same size as the float type");

  ParseResult parse(const uint8_t *&strm, unsigned long &N) override {
    // Parse the floating point as an integral type
    if (auto ret = int_buffer_.parse(strm, N); ret != ParseResult::SUCCESS)
      return ret;

    // Cast the buffer as a floating point value
    value_ = std::bit_cast<T>(int_buffer_.get());
    return ParseResult::SUCCESS;
  }

  T get() const { return value_; }

  inline void reset() override {
    value_ = 0;
    int_buffer_.reset();
  }

  // --------------------------------------------------------------------------
  // Internal members
  // --------------------------------------------------------------------------
private:
  T value_;
  IntegralParser<BufferType> int_buffer_;
};

using FloatByteParser = FloatingPointParser<float, uint32_t>;
using DoubleByteParser = FloatingPointParser<double, uint64_t>;

MK_BYTE_PARSER_WRAPPER(Tags::Float, FloatByteParser);
MK_BYTE_PARSER_WRAPPER(Tags::Double, DoubleByteParser);

} // namespace minecraft::nbt

#endif