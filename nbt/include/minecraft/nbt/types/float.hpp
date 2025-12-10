// ============================================================================
// Project: SOLISMC_FILEIO
//
// Definition of NBT floating types (float & double)
//
// Author    Meltwin (github@meltwin.fr)
// Date      28/11/2025 (created 28/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_TYPES_FLOAT_HPP
#define SOLISMC_NBT_TYPES_FLOAT_HPP

#include "minecraft/nbt/types/base.hpp"

namespace minecraft::nbt {

template <> constexpr Tags::E getTag<float>() { return Tags::Float; }
template <> constexpr Tags::E getTag<double>() { return Tags::Double; }

} // namespace minecraft::nbt

#endif