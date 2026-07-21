// ============================================================================
// Project: SOLISMC-FILEIO
//
// Compound object representation
//
// Author    Meltwin (github@meltwin.fr)
// Date      21/07/2026 (created 21/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLISMC_NBT_COMPOUND_HPP
#define SOLISMC_NBT_COMPOUND_HPP

#include "minecraft/io/nbt/field.hpp"
#include "minecraft/io/nbt/tag.hpp"
#include <string>
#include <unordered_map>

namespace minecraft::nbt {

/**
 * @brief Representation of
 *
 */
struct Compound : public std::unordered_map<std::string, FieldValue> {

  /**
   * @brief Set a new field in the compound
   *
   * @param name the name of the field
   * @param tag the NBT type tag of the field
   * @param value the value of this field
   */
  inline void set(const std::string &name, const Tag tag,
                  const FieldValue &value) {
    this->insert_or_assign(name, std::pair{tag, value});
  }

  /**
   * @brief Get the value of the given field
   */
  inline const FieldValue &get(const std::string &name) const {
    return this->at(name);
  }

  /**
   * @brief Get the type tag of the given field
   * @param name the name of the field
   */
  inline Tag get_field_tag(const std::string &name) const {
    return this->at(name).tag();
  }
};

} // namespace minecraft::nbt

#endif