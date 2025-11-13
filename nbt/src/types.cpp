// ============================================================================
// Project: SOLISMC_FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      13/11/2025 (created 13/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/nbt/types.hpp"

constexpr const char *
minecraft::NBTTags::getName(minecraft::NBTTags::Tags tag) {
  using minecraft::NBTTags;

#define MK_CASE(type)                                                          \
  case NBTTags::type:                                                          \
    return #type

  // Construct
  switch (tag) {
    MK_CASE(END);
    MK_CASE(Byte);
    MK_CASE(Short);
    MK_CASE(Int);
    MK_CASE(Long);
    MK_CASE(Float);
    MK_CASE(Double);
    MK_CASE(String);
    MK_CASE(ByteArray);
    MK_CASE(List);
    MK_CASE(IntArray);
    MK_CASE(LongArray);
    MK_CASE(Compound);
    MK_CASE(ERROR);
  }

#undef MK_TYPE
}