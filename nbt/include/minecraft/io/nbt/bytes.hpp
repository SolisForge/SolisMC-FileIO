// ============================================================================
// Project: SOLISMC-FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      04/06/2026 (created 04/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_IO_NBT_BYTE_PARSING
#define SOLISMC_IO_NBT_BYTE_PARSING

#include "minecraft/io/nbt/bytes/float.hpp"    // IWYU pragma: keep
#include "minecraft/io/nbt/bytes/integral.hpp" // IWYU pragma: keep
#include "minecraft/io/nbt/bytes/interface.hpp"
#include "minecraft/io/nbt/bytes/list.hpp"   // IWYU pragma: keep
#include "minecraft/io/nbt/bytes/string.hpp" // IWYU pragma: keep
#include "minecraft/io/nbt/bytes/struct.hpp" // IWYU pragma: keep

namespace minecraft::nbt {

// ------------------------------------------------------------------
// Default = JAVA specific NBT classes
// ------------------------------------------------------------------
template <typename T>
using ByteParser = byte::base::ByteParser<T, GameVersion::JAVA>;
template <typename T>
using ByteWriter = byte::base::ByteWriter<T, GameVersion::JAVA>;

// ------------------------------------------------------------------
// Bedrock specific NBT classes
// ------------------------------------------------------------------
namespace bedrock {

template <typename T>
using ByteParser = byte::base::ByteParser<T, GameVersion::BEDROCK>;
template <typename T>
using ByteWriter = byte::base::ByteWriter<T, GameVersion::BEDROCK>;
} // namespace bedrock

} // namespace minecraft::nbt

#endif