#ifndef NBT_READER_HPP
#define NBT_READER_HPP

/**
  ==================================== NBT ====================================

  This file contains the implementation of a NBT reader class.
  Based on the MC wiki:
    https://minecraft.wiki/w/NBT_format

  @author    Meltwin
  @date      05/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include <string>

namespace minecraft::nbt {

constexpr size_t CHUNK_SIZE{16384};

class NBTReader {
public:
  NBTReader();

  template <typename T>
  inline static T parse_file(const char *filename, bool compressed = true) {
    return parse_string<T>(get_file_content(filename), compressed);
  }

  template <typename T>
  static T parse_string(const std::string &stream, bool compressed = true);

  // ============================================================================
  //    IO related methods
  // ============================================================================
protected:
  /**
   * @brief Get the file content object
   *
   * @param filename
   */
  static std::string get_file_content(const char *filename);

  // ============================================================================
  //    Conversion related methods
  // ============================================================================
protected:
};

} // namespace minecraft::nbt

#endif