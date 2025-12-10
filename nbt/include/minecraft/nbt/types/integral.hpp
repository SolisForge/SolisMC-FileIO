// ============================================================================
// Project: SOLISMC_FILEIO
//
// Definition of NBT integral types (short, int, long, ...)
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_TYPES_INTEGRAL_HPP
#define SOLISMC_NBT_TYPES_INTEGRAL_HPP

#include "minecraft/nbt/types/base.hpp"
#include <cstdint>

namespace minecraft::nbt {

// ============================================================================
// Defining integral types
// ============================================================================

#define MK_INTEGRAL_TYPES(name, typename)                                      \
  template <> inline constexpr Tags::E getTag<typename>() {                    \
    return Tags::name;                                                         \
  }                                                                            \
  template <> inline constexpr Tags::E getTag<u##typename>() {                 \
    return Tags::name;                                                         \
  }                                                                            \
  using name = NBTTypeInfo<typename, Tags::name>;

MK_INTEGRAL_TYPES(Byte, int8_t);
MK_INTEGRAL_TYPES(Short, int16_t);
MK_INTEGRAL_TYPES(Int, int32_t);
MK_INTEGRAL_TYPES(Long, int64_t);

#undef MK_INTEGRAL_TYPES

} // namespace minecraft::nbt

#endif
