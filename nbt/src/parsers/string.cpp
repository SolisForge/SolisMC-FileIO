// ============================================================================
// Project: SOLISMC-FILEIO
//
// String byte-parsing implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/12/2025 (created 26/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parsers/string.hpp"
#include "minecraft/nbt/parsers/base.hpp"

namespace minecraft::nbt {

ParseResult BytesParser<std::string>::parse(const StreamChar *&strm,
                                            unsigned long &N) {
  // Reset parser if new parse
  if (size_parsed_ && parsed_)
    reset();

  // Read the length of the string
  if (!size_parsed_) {
    if (auto ret = size_parser_.parse(strm, N); ret != ParseResult::SUCCESS)
      return ret;
    size_parsed_ = true;
    value_.resize(size_parser_.get());
  }

  // Read string characters one by one
  NBT_PARSE_N_BYTE_BEGIN()
  value_[n_bytes] = strm[0];
  NBT_PARSE_N_BYTE_END(n_bytes, static_cast<std::size_t>(size_parser_.get()))
  parsed_ = true;
  return ParseResult::SUCCESS;
}

// Export for in-library compilation
template struct BytesParser<std::string>;

} // namespace minecraft::nbt