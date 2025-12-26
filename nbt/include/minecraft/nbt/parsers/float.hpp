// ============================================================================
// Project: SOLISMC-FILEIO
//
// Float types byte-parsing definition
//
// Author    Meltwin (github@meltwin.fr)
// Date      11/12/2025 (created 11/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_PARSER_FLOAT_HPP
#define SOLISMC_NBT_PARSER_FLOAT_HPP

#include "minecraft/nbt/parsers/integral.hpp" // IWYU pragma: keep
#include <concepts>
#include <cstdint>

namespace minecraft::nbt {

// ============================================================================
// Type adapter
// ============================================================================

/**
 * @brief Special structure to allow float -> int mapping
 */
template <typename T> struct FloatToInt {
  using INT_TYPE = void;
};
template <> struct FloatToInt<float> {
  using INT_TYPE = int32_t;
};
template <> struct FloatToInt<double> {
  using INT_TYPE = int64_t;
};

// ============================================================================
// Float parser
// ============================================================================

/**
 * @brief Parser implementation for floating point types
 */
template <std::floating_point T> struct BytesParser<T> {

  ParseResult parse(const StreamChar *&strm, unsigned long &N);

  T get() const { return parsed_ ? value_ : 0.0; }

  inline void reset() {
    value_ = 0;
    int_parser_.reset();
  }

private:
  T value_;
  BytesParser<typename FloatToInt<T>::INT_TYPE> int_parser_;
  bool parsed_ = false;
};

// ============================================================================
// Specialization definition
// ============================================================================
extern template struct BytesParser<float>;
extern template struct BytesParser<double>;

} // namespace minecraft::nbt

#endif