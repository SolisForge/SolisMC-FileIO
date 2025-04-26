#ifndef NBT_TYPES_HPP
#define NBT_TYPES_HPP

/**
  ==================================== NBT ====================================

  This file contains the description of NBT types.
  Based on the MC wiki:
    https://minecraft.wiki/w/NBT_format

  Follow version 19133 of NBT format

  @author    Meltwin
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

/**
 * @brief Base struct for NBT types
 */
struct NBTType {

  NBTType() {}

  ~NBTType() {
    // Delete allocated components
    if (name != nullptr)
      delete name;
  }

  inline const char *getName() { return name; }
  inline size_t getNameSize() { return name_length; }

public:
  char *name = nullptr;
  unsigned short name_length = 0;
};

// ============================================================================
// NBT primitives
// ============================================================================

/**
 * @brief Simple implementation of NBTType for primitive types.
 * @tparam T the corresponding C++ type
 */
template <typename T> struct NBTPrimitive : NBTType {
  T val;
  constexpr NBTPrimitive(T other) : val(other) {}
  constexpr NBTPrimitive() {}
  operator T() { return val; }
};

typedef NBTPrimitive<int8_t> Byte;     //!< Byte field
typedef NBTPrimitive<uint8_t> Boolean; //!< Boolean use a byte field
typedef NBTPrimitive<int16_t> Short;   //!< Short value field
typedef NBTPrimitive<int32_t> Int;     //!< Int value field
typedef NBTPrimitive<int64_t> Long;    //!< Long value field
typedef NBTPrimitive<float> Float;     //!< Float value field
typedef NBTPrimitive<double> Double;   //!< Double value field

/**
 * @brief String value field
 */
struct String : NBTPrimitive<char *> {
  uint16_t val_size;
};

// ============================================================================
// NBT Lists implementation
// ============================================================================

/**
 * @brief NBT list base interface
 * @tparam T the type of the list elements
 */
template <typename T> struct List : NBTType, std::vector<T> {};
typedef List<Byte> ByteArray; //!< List of bytes
typedef List<Int> IntArray;   //!< List of int
typedef List<Long> LongArray; //!< List of long

// ============================================================================
// NBT Object implementation
// ============================================================================

/**
 * @brief NBT general object.
 */
struct Compound : NBTType, std::map<String, NBTType *> {};

} // namespace minecraft::nbt

#endif