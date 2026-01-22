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
#include <cstdint>
#include <string>

namespace minecraft::nbt {

/**
 * @brief Parser implementation for strings
 */
template <> struct BytesParser<std::string> {

  ParseResult parse(const StreamChar *&, unsigned long &);

  std::string get() const { return is_parsed() ? value_ : EMPTY_STR; }

  /**
   * @brief Get the parsed length of the string or 0 if unfinished
   */
  uint16_t get_length() const { return size_parser_.get(); }

  inline void reset() {
    size_parser_.reset();
    n_bytes = 0;
    value_.resize(0);
    parsed_ = false;
    size_parsed_ = false;
  }

  inline bool is_parsed() const { return size_parsed_ && parsed_; }

private:
  static constexpr std::string EMPTY_STR{};
  std::string value_;
  BytesParser<uint16_t> size_parser_;
  std::size_t n_bytes;
  bool size_parsed_ = false;
  bool parsed_ = false;
};

// ============================================================================
// Specialization export in this library
// ============================================================================
extern template struct BytesParser<std::string>;

} // namespace minecraft::nbt

#endif