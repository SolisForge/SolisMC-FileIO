// ============================================================================
// Project: SOLISMC-FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/12/2025 (created 26/12/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parsers/string.hpp"

namespace minecraft::nbt {

template <>
ParseResult BytesParser<std::string>::parse(const StreamChar *&strm,
                                            unsigned long &N) {

  return ParseResult::SUCCESS;
}

// Export for in-library compilation
template struct BytesParser<std::string>;

} // namespace minecraft::nbt