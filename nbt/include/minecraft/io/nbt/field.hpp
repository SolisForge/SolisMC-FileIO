// ============================================================================
// Project: SOLISMC-FILEIO
//
// Type-agnostic field value object
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_BYTE_PARSING_FIELD_VALUE
#define SOLISMC_NBT_BYTE_PARSING_FIELD_VALUE
#include "minecraft/io/nbt/tag.hpp"
#include <any>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace minecraft::nbt {

// ============================================================================

/**
 * @brief Identity of a field
 */
struct FieldInfo {
  std::string name;
  Tag tag;
};

// ============================================================================
// Any value implementation
// ============================================================================

/**
 * @brief Object containing an object of any value
 *
 */
struct FieldValue {
  explicit FieldValue(const Tag tag, const std::any &v) : tag_(tag), value(v) {}
  ~FieldValue() = default;

  template <typename C> const C as() const { return std::any_cast<C>(value); }

  inline Tag tag() const { return tag_; }

protected:
  Tag tag_;
  std::any value;
};

} // namespace minecraft::nbt
#endif