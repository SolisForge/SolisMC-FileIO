#ifndef NBT_TYPES_HPP
#define NBT_TYPES_HPP

/**
  ==================================== NBT ====================================

  This file contains the description of NBT types.
  Based on the MC wiki:
    https://minecraft.wiki/w/NBT_format

  @author    Geoffrey Côte
  @date      05/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace minecraft::nbt {

struct NBTType {};
template <typename T> struct NBTPrimitive : NBTType {
  T val;
  constexpr NBTPrimitive(T other) : val(other) {}
  operator T() { return val; }
};

// Declaration of NBT primitives
typedef NBTPrimitive<int8_t> Byte;        /// Byte field
typedef NBTPrimitive<uint8_t> Boolean;    /// Boolean use a byte field
typedef NBTPrimitive<int16_t> Short;      /// Short value field
typedef NBTPrimitive<int32_t> Int;        /// Int value field
typedef NBTPrimitive<int64_t> Long;       /// Long value field
typedef NBTPrimitive<float> Float;        /// Float value field
typedef NBTPrimitive<double> Double;      /// Double value field
typedef NBTPrimitive<std::string> String; /// String value field
template <typename T> struct AbstractList : NBTType, std::vector<T> {};
typedef AbstractList<NBTType *> List; // Vector field
struct Compound : NBTType, std::map<String, NBTType *> {};

// List implementations
typedef AbstractList<Byte> ByteArray;
typedef AbstractList<Int> IntArray;
typedef AbstractList<Long> LongArray;

} // namespace minecraft::nbt

#endif