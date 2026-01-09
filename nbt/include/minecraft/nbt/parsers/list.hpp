// ============================================================================
// Project: SOLISMC-FILEIO
//
// List byte-parsing definition
//
// Author    Meltwin (github@meltwin.fr)
// Date      09/01/2026 (created 09/01/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_PARSER_LIST_HPP
#define SOLISMC_NBT_PARSER_LIST_HPP

#include "minecraft/nbt/parsers/integral.hpp"
#include <bitset>
#include <cstdint>
#include <memory>
#include <vector>

namespace minecraft::nbt {

/**
 * @brief Parser implementation for std::vector
 */
template <typename T> struct BytesParser<std::vector<T>> {

  ParseResult parse(const StreamChar *&, unsigned long &);

  /**
   * @brief Get a pointer to the parsed vector if the parsing was complete,
   * nullptr otherwise.
   */
  std::shared_ptr<std::vector<T>> get() const {
    return parsed_[0] && parsed_[1] ? p_value_ : nullptr;
  }

  inline void reset() {
    size_parser_.reset();
    elem_parser_.reset();
    p_value_.reset();
    p_value_ = nullptr;
    parsed_.reset();
    n_elements_expected_ = 0;
    n_elements_parsed_ = 0;
  }

private:
  BytesParser<int32_t> size_parser_;
  BytesParser<T> elem_parser_;
  std::shared_ptr<std::vector<T>> p_value_ = nullptr;
  uint32_t n_elements_parsed_ = 0;
  uint32_t n_elements_expected_ = 0;

  // Parsing flags
  std::bitset<2> parsed_{0x0};
};

// Export for in-library compilation
extern template struct BytesParser<std::vector<int8_t>>;  // nbt::ByteArray
extern template struct BytesParser<std::vector<int32_t>>; // nbt::IntArray
extern template struct BytesParser<std::vector<int64_t>>; // nbt::LongArray

} // namespace minecraft::nbt

#endif