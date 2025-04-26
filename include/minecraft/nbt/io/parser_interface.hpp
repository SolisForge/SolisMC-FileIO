#ifndef NBT_IO_PARSER_INTERFACE_HPP
#define NBT_IO_PARSER_INTERFACE_HPP

/**
  ==================================== NBT ====================================

  This file contains the parsing interface of NBT streams.
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
#include <solis/utils/types.hpp>


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
  bool check_tag_id(NBT_STREAM_STD_ARGS, NBTTags::Tags tag);

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
  ParseResult parse_type_header(NBT_STREAM_STD_ARGS, NBTTags::Tags tag,
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

  /**
   * @brief Read an integral-type value from the type (i.e. byte, short, int,
   * long, etc)
   *
   * The NBT format stores the values as big-endian.
   *
   * @tparam T
   * @tparam NB
   * @param dest
   * @return ParseResult
   */
  template <typename T, uint8_t NB = sizeof(T)>
  inline ParseResult read_integral(_NBT_STREAM_STRM_ARGS, T &dest) {
    dest = 0;
    NBT_PARSE_N_BYTE_BEGIN()
#if SOLIS_BIG_ENDIAN == 1
    dest += ((T)(*strm)[0]);
    if (read_bytes < NB - 1)
      dest = dest << BIT_PER_BYTE;
#else
    dest += ((T)(*strm)[0]) << read_bytes * BIT_PER_BYTE;
#endif
    NBT_PARSE_N_BYTE_END(NB)
    read_bytes = 0;
    return ParseResult::SUCCESS;
  }

  /**
   * @brief Parse a floating point
   *
   * @return ParseResult
   */
  template <typename T, typename B, uint8_t NB = sizeof(T)>
  ParseResult read_floating(_NBT_STREAM_STRM_ARGS, T &dest, B &buffer) {
    static_assert(
        sizeof(T) == sizeof(B),
        "Floating point T and buffer B should have the same byte size!");
    if (auto ret = read_integral<B, NB>(strm, N, buffer);
        ret != ParseResult::SUCCESS)
      return ret;

    // Convert it to a float value
    dest = *reinterpret_cast<T *>(&buffer);
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, int8_t &dest) {
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, uint8_t &dest) {
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, int16_t &dest) {
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, uint16_t &dest) {
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, int32_t &dest) {
    return read_integral<int32_t, 4>(strm, N, dest);
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, uint32_t &dest) {
    return read_integral<uint32_t, 4>(strm, N, dest);
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, int64_t &dest) {
    return read_integral<int64_t, 8>(strm, N, dest);
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, uint64_t &dest) {
    return read_integral<uint64_t, 8>(strm, N, dest);
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
  inline ParseResult read(_NBT_STREAM_STRM_ARGS, char **dest, uint32_t size) {
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
// Base templated NBT parser
// ############################################################################

/**
 * @brief Parser dedicated for the NBT types.
 *
 * @tparam T the NBT type this parser will parse
 */
template <typename T, NBTTags::Tags E = Tag<T>()>
struct NBTTypeParser : _INBTTypeParser {

  ParseResult parse(NBT_STREAM_PARSE_HEADER) override {
    // If already parsed, stop
    if (has_flag(flags, ParseFlags::PAYLOAD))
      return ParseResult::SUCCESS;

    if (curr == nullptr)
      curr = initialize_var();

    // Parse the header
    if (auto ret = parse_type_header(NBT_STREAM_FWD_ARGS, E, curr);
        ret != ParseResult::SUCCESS)
      return ret;

    // Parse value
    return parse_payload(strm, N);
  }

  T *getParsed() { return curr; }

  /**
   * @brief Initialize a NBT type pointer
   * @return a pointer of the type of the parser
   */
  virtual T *initialize_var() { return new T(); }

protected:
  /**
   * @brief Parse the payload of the given type
   */
  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS);

protected:
  T *curr = nullptr;
};

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
  template <typename T, NBTTags::Tags E> static NBTTypeParser<T, E> make() {
    static_assert(std::is_base_of_v<NBTType, T>,
                  "T should be a child of NBTType");
    return NBTTypeParser<T, E>();
  }
};

} // namespace minecraft::nbt

#endif