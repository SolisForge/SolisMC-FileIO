// ============================================================================
// Project: SOLISMC-FILEIO
//
// Adapters for object loading & saving
//
// Author    Meltwin (github@meltwin.fr)
// Date      15/07/2026 (created 15/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_BYTE_PARSING_STRUCT_ADAPTER
#define SOLISMC_NBT_BYTE_PARSING_STRUCT_ADAPTER

#include "minecraft/io/nbt/bytes/field.hpp"
#include "minecraft/io/nbt/bytes/interface.hpp"

namespace minecraft::nbt::byte::base {

// ============================================================================
enum class FieldState : uint8_t { EXIST = 0, WRONG_TYPE = 1, DO_NOT_EXIST = 2 };

// ============================================================================
// Adapter interfaces
// ============================================================================
struct LoadAdapter {
  virtual ~LoadAdapter() = default;

  /**
   * @brief Get the byte parser needed for the given field
   */
  virtual std::unique_ptr<ByteParserInterface>
  get_byte_parser(const FieldInfo &) const = 0;

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

template <typename T>
concept WriterAdapterImplementation = std::is_base_of_v<LoadAdapter, T>;

// ============================================================================
struct SaveAdapter {
  virtual ~SaveAdapter() = default;

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

} // namespace minecraft::nbt::byte::base

#endif