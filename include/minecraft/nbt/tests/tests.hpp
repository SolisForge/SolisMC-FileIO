#ifndef NBT_TEST_HPP
#define NBT_TEST_HPP

/**
  ==================================== NBT ====================================

  This file contains methods and classes used for testing.

  @author    Meltwin
  @date      04/05/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "minecraft/nbt/io/parser_interface.hpp"
#include <cstring>
#include <doctest.h>

namespace minecraft::nbt {

// ============================================================================
// Template declaration
// ============================================================================

struct IExpected {
  ParseResult result;
  size_t name_size;
  const char *name;
};

template <typename T, typename V = void> struct Expected;

// ============================================================================
// Implementation of Expected for the several NBT types
// ============================================================================

template <typename T>
struct Expected<T, typename std::enable_if<
                       solis::is_specialization<T, NBTPrimitive>::value>::type>
    : IExpected {
  typename T::CType value;

  /**
   * @brief Compare NBT primitive to the expected result
   *
   * @param r the result of the parsing
   * @param got the value object that the parsing gave
   */
  inline void compare_to(ParseResult r, T *got) {
    CHECK(r == result);
    CHECK(got->getNameSize() == name_size);
    CHECK(strncmp(got->getName(), name, got->getNameSize()) == 0);
    CHECK(got->val == value);
  }
};

/**
 * @brief Compare NBT String to the expected result
 *
 * @param r the result of the parsing
 * @param got the string object that the parsing gave
 */
template <> struct Expected<String> : IExpected {
  size_t value_size;
  const char *value;

  inline void compare_to(ParseResult r, String *got) {
    CHECK(r == result);
    CHECK(got->getNameSize() == name_size);
    CHECK(strncmp(got->getName(), name, got->getNameSize()) == 0);
    CHECK(got->val_size == value_size);
    CHECK(strncmp(got->val, value, got->val_size) == 0);
  }
};

template <typename T>
struct Expected<
    T, typename std::enable_if<solis::is_specialization<T, List>::value>::type>
    : IExpected {
  int32_t vector_size;
  std::vector<typename T::CType> vector;

  /**
   * @brief Compare NBT list to the expected result
   *
   * @param r the result of the parsing
   * @param got the value object that the parsing gave
   */
  inline void compare_to(ParseResult r, T *got) {
    CHECK(r == result);
    CHECK(got->getNameSize() == name_size);
    CHECK(strncmp(got->getName(), name, got->getNameSize()) == 0);
    CHECK(got->size() == vector_size);
    for (uint32_t i = 0; i < got->size(); i++)
      CHECK((*got)[i] == vector[i]);
  }
};

} // namespace minecraft::nbt

#endif