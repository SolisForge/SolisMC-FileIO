// ============================================================================
// Project: SOLISMC-FILEIO
//
// Integral types byte-parsing definition
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/12/2025 (created 26/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_PARSER_INTEGRAL_HPP
#define SOLISMC_NBT_PARSER_INTEGRAL_HPP

#include "minecraft/nbt/parsers/base.hpp"
#include <concepts>

namespace minecraft::nbt {

/**
 * @brief ByteParser specialization for integral types.
 */
template <std::integral T> struct BytesParser<T> {

  ParseResult parse(const StreamChar *&strm, unsigned long &N);

  inline T get() const { return is_parsed() ? value_ : 0; }

  inline void reset() {
    value_ = 0;
    n_bytes = 0;
  }

  inline bool is_parsed() const { return n_bytes == 0; }

private:
  T value_;
  std::size_t n_bytes = 0;
  static constexpr auto TYPE_LENGTH{sizeof(T)};
};

// ============================================================================
// Specialization definition
// ============================================================================
extern template struct BytesParser<int8_t>;
extern template struct BytesParser<uint8_t>;
extern template struct BytesParser<int16_t>;
extern template struct BytesParser<uint16_t>;
extern template struct BytesParser<int32_t>;
extern template struct BytesParser<uint32_t>;
extern template struct BytesParser<int64_t>;
extern template struct BytesParser<uint64_t>;

} // namespace minecraft::nbt

#endif