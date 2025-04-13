#include "minecraft/nbt/format/tags.hpp"

namespace minecraft::nbt {

// ============================================================================
//    To ENUM runtime conversion
// ============================================================================

NBTTags Tag(uint8_t id) {
#define GENERATE_CASE(id)                                                      \
  case id:                                                                     \
    return Tag<id>();

  switch (id) {
    GENERATE_CASE(1)
    GENERATE_CASE(2)
    GENERATE_CASE(3)
    GENERATE_CASE(4)
    GENERATE_CASE(5)
    GENERATE_CASE(6)
    GENERATE_CASE(7)
    GENERATE_CASE(8)
    GENERATE_CASE(9)
    GENERATE_CASE(10)
    GENERATE_CASE(11)
    GENERATE_CASE(12)
  default:
    return NBTTags::END;
  }
#undef GENERATE_CASE
}

// ============================================================================
//    To INT runtime conversion
// ============================================================================

uint8_t TagID(NBTTags t) {
#define GENERATE_CASE(type)                                                    \
  case NBTTags::type:                                                          \
    return TagID<NBTTags::type>();

  switch (t) {
    GENERATE_CASE(Byte)
    GENERATE_CASE(Short)
    GENERATE_CASE(Int)
    GENERATE_CASE(Long)
    GENERATE_CASE(Float)
    GENERATE_CASE(Double)
    GENERATE_CASE(ByteArray)
    GENERATE_CASE(String)
    GENERATE_CASE(List)
    GENERATE_CASE(Compound)
    GENERATE_CASE(IntArray)
    GENERATE_CASE(LongArray)
  default:
    return TagID<NBTTags::END>();
  }
#undef GENERATE_CASE
}

// ============================================================================
//    To STR runtime conversion
// ============================================================================

const char *TagStr(NBTTags t) {
#define GENERATE_CASE(type)                                                    \
  case NBTTags::type:                                                          \
    return TagStr<NBTTags::type>();

  switch (t) {
    GENERATE_CASE(Byte)
    GENERATE_CASE(Short)
    GENERATE_CASE(Int)
    GENERATE_CASE(Long)
    GENERATE_CASE(Float)
    GENERATE_CASE(Double)
    GENERATE_CASE(ByteArray)
    GENERATE_CASE(String)
    GENERATE_CASE(List)
    GENERATE_CASE(Compound)
    GENERATE_CASE(IntArray)
    GENERATE_CASE(LongArray)
  default:
    return TagStr<NBTTags::END>();
  }
#undef GENERATE_CASE
}

} // namespace minecraft::nbt