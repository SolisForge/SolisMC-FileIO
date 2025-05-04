#ifndef NBT_IO_PARSER_ARRAY_HPP
#define NBT_IO_PARSER_ARRAY_HPP

/**
  ==================================== NBT ====================================

  This file contains the definition of the arrays parsing of NBT streams.
  Based on the MC wiki:
    https://minecraft.wiki/w/NBT_format

  Follow version 19133 of NBT format

  @author    Meltwin
  @date      17/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "minecraft/nbt/io/parser_interface.hpp"

namespace minecraft::nbt {

// ############################################################################
// Array Parsers
// ############################################################################

/**
 * @brief Parser specialization for fixed size arrays
 *
 * @tparam T the NBT type
 */
template <typename T> struct INBTArrayParser : public NBTTypeParser<List<T>> {

  INBTArrayParser<T>() = default;

  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS) override {
    // Parse list size
    if (auto ret = this->read(strm, N, list_size); ret != ParseResult::SUCCESS)
      return ret;

    for (int32_t i = parsed; i < list_size; i++) {
      if (auto ret = parse_item(strm, N); ret != ParseResult::SUCCESS)
        return ret;
      this->curr->push_back(parsing_val);
      parsed++;
    }

    return ParseResult::SUCCESS;
  }

  /**
   * @brief Parse the list item based from the stream
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  virtual ParseResult parse_item(_NBT_STREAM_STRM_ARGS);

public:
  int32_t list_size = 0;

  // Item parsing temporary properties
protected:
  T parsing_val;
  int32_t parsed = 0;
};

typedef INBTArrayParser<int8_t> NBTByteArrayParser;
typedef INBTArrayParser<int32_t> NBTIntArrayParser;
typedef INBTArrayParser<int64_t> NBTLongArrayParser;
// typedef INBTArrayParser<List *> NBTListParser;

} // namespace minecraft::nbt

#endif