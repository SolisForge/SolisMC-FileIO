// ============================================================================
// Project: SOLISMC-FILEIO
//
// String byte-parsing definition
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/12/2025 (created 26/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_PARSER_STRING_HPP
#define SOLISMC_NBT_PARSER_STRING_HPP

#include "minecraft/nbt/parsers/integral.hpp"
#include <string>

namespace minecraft::nbt {

/**
 * @brief Parser implementation for strings
 */
template <> struct BytesParser<std::string> {

  ParseResult parse(const StreamChar *&, unsigned long &);

  std::string get() const { return parsed_ ? value_ : EMPTY_STR; }

  inline void reset() {
    size_parser_.reset();
    n_bytes = 0;
    value_ = std::string(nullptr);
  }

private:
  static constexpr std::string EMPTY_STR{""};
  std::string value_;
  BytesParser<int16_t> size_parser_;
  std::size_t n_bytes;
  bool parsed_;
};

// ============================================================================
// Specialization export in this library
// ============================================================================
extern template struct BytesParser<std::string>;

} // namespace minecraft::nbt

#endif