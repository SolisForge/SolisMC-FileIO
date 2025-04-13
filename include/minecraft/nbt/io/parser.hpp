#ifndef NBT_IO_PARSER_HPP
#define NBT_IO_PARSER_HPP

/**
  ==================================== NBT ====================================

  This file contains the parsing of NBT streams.
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

#include "minecraft/nbt/format/tags.hpp"
#include "minecraft/nbt/format/types.hpp"
#include <cstddef>
#include <solis/utils/flags.hpp>
#include <solis/utils/output.hpp>

#include <iostream>

namespace minecraft::nbt {

using solis::has_flag;

// ##########################################################################
// Parsing utils
// ##########################################################################

/**
 * Possible results of parsing:
 *  - SUCCESS when parsing successfully finished
 *  - UNFINISHED when the parser is missing some bytes
 *  - FAIELD when something went wrong and the parser can't finish
 */
enum class ParseResult : uint8_t { SUCCESS, UNFINISHED, FAILED };

/**
 * @brief Standard parsing flag for NBT tags.
 * Custom ones can be added to these ones if needed
 */
struct ParseFlags {
  CFLAG(NONE, 0b00000000)
  CFLAG(ID, 0b00000001)
  CFLAG(NAME_SIZE, ID << 1)
  CFLAG(NAME, NAME_SIZE << 1)
  CFLAG(PAYLOAD_SIZE, NAME_SIZE << 1)
  CFLAG(PAYLOAD, PAYLOAD_SIZE << 1)
};

// -------------------------------- Args utils --------------------------------
#define _NBT_STREAM_STRM_ARGS                                                  \
  [[maybe_unused]] unsigned char **strm, [[maybe_unused]] size_t &N
#define NBT_STREAM_STD_ARGS                                                    \
  _NBT_STREAM_STRM_ARGS, [[maybe_unused]] bool include_head
#define NBT_STREAM_FWD_ARGS strm, N, include_head
#define NBT_STREAM_PARSE_HEADER NBT_STREAM_STD_ARGS = true

// ------------------------------- Parsing utils ------------------------------
#define NBT_PARSE_N_BYTE_BEGIN()                                               \
  if (N <= 0)                                                                  \
    return ParseResult::UNFINISHED;                                            \
  do {
#define NBT_PARSE_N_BYTE_END(len)                                              \
  read_bytes++;                                                                \
  inc_stream(strm, N);                                                         \
  }                                                                            \
  while (N > 0 && read_bytes < len)                                            \
    ;                                                                          \
  if (read_bytes != len) {                                                     \
    return ParseResult::UNFINISHED;                                            \
  }

// ############################################################################
// NBT Parsing interface
// ############################################################################

/**
 * @brief Interface for the NBT parsers.
 */
struct _INBTTypeParser {
  static constexpr uint8_t BIT_PER_BYTE = 8;

  /**
   * @brief Parse the value from the stream.
   * @return a result state.
   */
  virtual ParseResult parse(NBT_STREAM_PARSE_HEADER) = 0;

  // ==========================================================================
  // General parsing utilities
  // ==========================================================================
protected:
  /**
       * @brief Read the name of the nbt tag.
       *
       * @param strm the stream to read from
       * @param N the number of bytes in the stream
       * @return a parse result, SUCCESS if parsing finished properly,
       UNFINISHED if
       * parsing has not been completed due to lack of data and FAILED
       otherwise
       */
  ParseResult read_name(NBT_STREAM_STD_ARGS, NBTType *dest);

  /**
   * @brief Read the name size of the nbt tag.
   *
   * @param strm the stream to read from
   * @param N the number of bytes in the stream
   * @return a parse result, SUCCESS if parsing finished properly,
   UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED
   otherwise
   */
  ParseResult read_name_size(NBT_STREAM_STD_ARGS, NBTType *dest);

  /**
   * @brief Check if the first byte correspond to the type id.
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @return true if the tag is the same
   * @return false otherwise
   */
  bool check_tag_id(NBT_STREAM_STD_ARGS, uint8_t tag);

  /**
   * @brief Parse the header of the tag (check type & name)
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param tag for testing the tag
   * @return a parse result, SUCCESS if parsing finished properly,
   UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED
   otherwise
   */
  ParseResult parse_type_header(NBT_STREAM_STD_ARGS, uint8_t tag,
                                NBTType *dest);

  // ==========================================================================
  // Stream manipulation and basic conversions
  // ==========================================================================
protected:
  /**
   * @brief Modify both stream and left counter.
   * @param inc the increment
   */
  inline void inc_stream(_NBT_STREAM_STRM_ARGS, uint32_t inc = 1) {
    (*strm) += inc;
    N -= inc;
  }

  template <typename T, uint8_t NB = sizeof(T)>
  inline ParseResult read_integral(_NBT_STREAM_STRM_ARGS, T &dest) {
    dest = 0;
    NBT_PARSE_N_BYTE_BEGIN()
    if (read_bytes < NB - 1)
      dest += ((T)(*strm)[0]) << BIT_PER_BYTE;
    else if (read_bytes == 1)
      dest += ((T)(*strm)[0]);
    NBT_PARSE_N_BYTE_END(NB)
    read_bytes = 0;
    return ParseResult::SUCCESS;
  }

  /**
   * @brief Parse the next two bytes in the stream as a short value
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param dest the destination short variable
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  inline ParseResult read_short(_NBT_STREAM_STRM_ARGS, int16_t &dest) {
    return read_integral(strm, N, dest);
  }

  /**
   * @brief Parse the next two bytes in the stream as an unsigned short value
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param dest the destination unsigned short variable
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  inline ParseResult read_short(_NBT_STREAM_STRM_ARGS, uint16_t &dest) {
    return read_integral(strm, N, dest);
  }

  /**
   * @brief Parse the next four bytes in the stream as a int value
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param dest the destination int variable
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  inline ParseResult read_int(_NBT_STREAM_STRM_ARGS, int32_t &dest) {
    return read_integral(strm, N, dest);
  }

  /**
   * @brief Parse the next four bytes in the stream as an unsigned int value
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param dest the destination unsigned int variable
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  inline ParseResult read_int(_NBT_STREAM_STRM_ARGS, uint32_t &dest) {
    return read_integral(strm, N, dest);
  }

  /**
   * @brief Parse the next eight bytes in the stream as a long value
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param dest the destination long variable
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  inline ParseResult read_long(_NBT_STREAM_STRM_ARGS, int64_t &dest) {
    return read_integral(strm, N, dest);
  }

  /**
   * @brief Parse the next eight bytes in the stream as an unsigned long value
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param dest the destination unsigned long variable
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  inline ParseResult read_long(_NBT_STREAM_STRM_ARGS, uint64_t &dest) {
    return read_integral(strm, N, dest);
  }

  /**
   * @brief Parse the next two bytes in the stream as a short value
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param dest the destination short variable
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  inline ParseResult read_string(_NBT_STREAM_STRM_ARGS, char **dest,
                                 uint32_t size) {
    // Initialize char array if needed
    if (*dest == nullptr)
      (*dest) = new char[size];

    // Parse bytes
    NBT_PARSE_N_BYTE_BEGIN()
    (*dest)[read_bytes] = (*strm)[0];
    NBT_PARSE_N_BYTE_END(size)
    (*dest)[read_bytes] = '\0';
    read_bytes = 0;
    return ParseResult::SUCCESS;
  }

  // ==========================================================================
  // Parser properties
  // ==========================================================================
protected:
  uint32_t read_bytes = 0;
  solis::FlagC_t flags = ParseFlags::NONE;
};

// ############################################################################
// Specialized NBT parser
// ############################################################################

/**
 * @brief Parser dedicated for the NBT types.
 *
 * @tparam T the NBT type this parser will parse
 */
template <typename T, NBTTags E> struct NBTTypeParser : _INBTTypeParser {

  ParseResult parse(NBT_STREAM_PARSE_HEADER) override {
    // If already parsed, stop
    if (has_flag(flags, ParseFlags::PAYLOAD))
      return ParseResult::SUCCESS;

    if (curr == nullptr)
      curr = new T();

    // Parse the header
    if (auto ret = parse_type_header(NBT_STREAM_FWD_ARGS, TagID(E), curr);
        ret != ParseResult::SUCCESS)
      return ret;

    // Parse value
    return parse_payload(strm, N);
  }

  T *getParsed() { return curr; }

protected:
  /**
   * @brief Parse the payload of the given type
   */
  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS);

protected:
  T *curr = nullptr;
};

// ----------------------------- Primitives Parsers ---------------------------

#define NBT_PARSER_DEF(t) typedef NBTTypeParser<t, NBTTags::t> NBT##t##Parser;

NBT_PARSER_DEF(Byte)
typedef NBTTypeParser<Boolean, NBTTags::Byte> NBTBooleanParser;
NBT_PARSER_DEF(Short)
NBT_PARSER_DEF(Int)
NBT_PARSER_DEF(Long)

// -------------------------- Float / Double Parsers --------------------------
struct NBTFloatParser : NBTTypeParser<Float, NBTTags::Float> {

  /**
   * @brief Parse the payload of the given type
   */
  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS) override;

protected:
  uint32_t _bytes; //!< Int buffer for storing float bytes
};

struct NBTDoubleParser : NBTTypeParser<Double, NBTTags::Double> {

  /**
   * @brief Parse the payload of the given type
   */
  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS) override;

protected:
  uint64_t _bytes; //!< Long buffer for storing double bytes
};

// ------------------------------ Complex Parsers -----------------------------
struct NBTStringParser : NBTTypeParser<String, NBTTags::String> {
protected:
  /**
   * @brief Parse the payload of the given type
   */
  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS) override;
};
NBT_PARSER_DEF(ByteArray)
NBT_PARSER_DEF(IntArray)
NBT_PARSER_DEF(LongArray)
NBT_PARSER_DEF(Compound)

#undef NBT_PARSER_DEF

// ############################################################################
// General NBT Parser
// ############################################################################

/**
 * @brief General purpose parser, also act as a builder for the NBTTypeParsers.
 */
struct NBTParser {
  /**
   * @brief Make a parser for the NBT type T
   *
   * @tparam T the NBT type to parse
   * @return a specialized parser
   */
  template <typename T, NBTTags E> static NBTTypeParser<T, E> make() {
    static_assert(std::is_base_of_v<NBTType, T>,
                  "T should be a child of NBTType");
    return NBTTypeParser<T, E>();
  }
};

} // namespace minecraft::nbt

#endif