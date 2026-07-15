// ============================================================================
// Project: SOLISMC-FILEIO
//
// C++ struct import/export logic
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_BYTE_PARSING_STRUCT
#define SOLISMC_NBT_BYTE_PARSING_STRUCT
#include "minecraft/io/nbt/bytes/field_value.hpp"
#include "minecraft/io/nbt/tag.hpp"
#include <iterator>
#include <string>
#include <vector>

namespace minecraft::nbt::byte {

// ============================================================================
// Field information
// ============================================================================
struct FieldInfo {
  std::string name;
  Tag tag;
};

// ============================================================================
enum class FieldState : uint8_t { EXIST = 0, WRONG_TYPE = 1, DO_NOT_EXIST = 2 };

// ============================================================================
// Adapter interfaces
// ============================================================================
struct WriteAdapter {
  virtual ~WriteAdapter() = default;

  /** Does the field exist in this class */
  virtual FieldState is_field(const FieldInfo &info) const noexcept = 0;

  /**
   * @brief Set the value to the field
   *
   * @param info which field to set
   * @param value a pointer to the value to set
   */
  virtual void set_field(const FieldInfo &info, const FieldValue &value) = 0;
};

// ============================================================================
struct ReadAdapter {
  virtual ~ReadAdapter() = default;

  /** Does the field exist in this class */
  virtual std::iterator_traits<FieldInfo> &iter_fields() const noexcept = 0;

  /**
   * @brief Set the value to the field
   *
   * @param info which field to set
   * @param value a pointer to the value to set
   */
  virtual void *get_field(const FieldInfo &info) = 0;
};

} // namespace minecraft::nbt::byte
#endif