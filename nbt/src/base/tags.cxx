// ============================================================================
// Project: SOLISMC_IO
//
// NBT data type tags utilities implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      02/09/2026 (created 02/09/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/tags.hxx"

namespace minecraft::nbt {

const char *to_str(Tags const tag) {
  // X macro definition
#define X(t)                                                                   \
  case Tags::t:                                                                \
    return #t;

  // Switch statement
  switch (tag) {
    // Generator
#include ".xmacros/tags.xmacro"

  default:
    return "UNKNOWN";
  }
#undef X
};

} // namespace minecraft::nbt