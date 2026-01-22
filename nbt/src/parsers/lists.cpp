// ============================================================================
// Project: SOLISMC-FILEIO
//
// List types byte-parsing implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      09/01/2026 (created 09/01/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parsers/base.hpp"
#include "minecraft/nbt/parsers/list.hpp"
#include <memory>

namespace minecraft::nbt {

template <typename T>
ParseResult BytesParser<std::vector<T>>::parse(const StreamChar *&strm,
                                               unsigned long &N) {
  // Reset before starting a new parsing
  if (parsed_[0] && parsed_[1])
    reset();

  // Parse vector length
  if (!parsed_[0]) {
    if (auto ret = size_parser_.parse(strm, N); ret != ParseResult::SUCCESS)
      return ret;

    // Create a vector of expected size
    n_elements_expected_ = static_cast<uint32_t>(size_parser_.get());
    p_value_ = std::make_shared<std::vector<T>>(n_elements_expected_);
    parsed_[0] = true;
  }

  // Parse vector elements
  while (n_elements_parsed_ < n_elements_expected_) {
    if (auto ret = elem_parser_.parse(strm, N); ret != ParseResult::SUCCESS)
      return ret;

    (*p_value_)[n_elements_parsed_] = elem_parser_.get();
    n_elements_parsed_++;
  }

  // Turn completion flag on
  parsed_[1] = true;
  return ParseResult::SUCCESS;
}

// Export for in-library compilation
template struct BytesParser<std::vector<int8_t>>;  // nbt::ByteArray
template struct BytesParser<std::vector<int32_t>>; // nbt::IntArray
template struct BytesParser<std::vector<int64_t>>; // nbt::LongArray

} // namespace minecraft::nbt