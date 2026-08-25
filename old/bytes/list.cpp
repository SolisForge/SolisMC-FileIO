// ============================================================================
// Project: SOLISMC-FILEIO
//
// NBT list parser & writer implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      04/06/2026 (created 04/06/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/list.hpp" // IWYU pragma: keep
#include <cmath>

namespace minecraft::nbt::byte::base {

// ============================================================================
// Force definition of these ByteParser in this library
// ============================================================================
EXPORT_COMMON_NBT_PARSER(GameVersion::JAVA, std::vector<int8_t>,
                         std::vector<int32_t>, std::vector<int64_t>);
EXPORT_COMMON_NBT_PARSER(GameVersion::BEDROCK, std::vector<int8_t>,
                         std::vector<int32_t>, std::vector<int64_t>);

} // namespace minecraft::nbt::byte::base
