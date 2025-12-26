// ============================================================================
// Project: SOLISMC-FILEIO
//
// Float types byte-parsing implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/12/2025 (created 26/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/nbt/parsers/float.hpp"
#include <bit>

namespace minecraft::nbt {

template <std::floating_point T>
ParseResult BytesParser<T>::parse(const StreamChar *&strm, unsigned long &N) {
  // Parse the floating point as an integral type
  parsed_ = false;
  if (auto ret = int_parser_.parse(strm, N); ret != ParseResult::SUCCESS)
    return ret;

  // Cast the buffer as a floating point value
  value_ = std::bit_cast<T>(int_parser_.get());
  parsed_ = true;
  return ParseResult::SUCCESS;
}

template struct BytesParser<float>;
template struct BytesParser<double>;

} // namespace minecraft::nbt