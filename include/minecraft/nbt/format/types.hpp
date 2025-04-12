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
#include <solis/utils/flags.hpp>
#include <string>
#include <vector>

namespace minecraft::nbt {

#define NBT_STREAM_STD_ARGS                                                    \
  [[maybe_unused]] unsigned char *strm, [[maybe_unused]] size_t &N
#define NBT_STREAM_STD_ARGS_INNER                                              \
  [[maybe_unused]] unsigned char **strm, [[maybe_unused]] size_t &N
#define NBT_STREAM_PARSE_HEADER                                                \
  parse_stream(NBT_STREAM_STD_ARGS, bool append = true)
#define NBT_STREAM_PARSE_IMPL                                                  \
  parse_stream(NBT_STREAM_STD_ARGS, [[maybe_unused]] bool append)

enum class ParseResult : uint8_t { SUCCESS, UNFINISHED, FAILED };
enum ParseFlags : solis::FlagC_t {
  NONE = 0x0,
  ID = 0x1,
  NAME_SIZE = 0x2,
  NAME = 0x4,
  PAYLOAD = 0x8
};

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

  /**
   * @brief Parse a given stream for the type.
   * The first byte of the given stream will always be on the tag ID byte.
   *
   * @param strm the stream to parse
   * @param N the maximum number of bytes to read
   * @param append if the read was unfinished, continue where where left of
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  virtual ParseResult NBT_STREAM_PARSE_HEADER = 0;

  inline const char *getName() { return name; }
  inline size_t getNameSize() { return name_length; }

  // ##########################################################################
  // Parsing methods
  // ##########################################################################
protected:
  /**
   * @brief Read the name of the nbt tag.
   *
   * @param strm the stream to read from
   * @param N the number of bytes in the stream
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  ParseResult read_name(NBT_STREAM_STD_ARGS_INNER);

  /**
   * @brief Read the name size of the nbt tag.
   *
   * @param strm the stream to read from
   * @param N the number of bytes in the stream
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  ParseResult read_name_size(NBT_STREAM_STD_ARGS_INNER);

  /**
   * @brief Check if the first byte correspond to the type id.
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @return true if the tag is the same
   * @return false otherwise
   */
  bool check_tag_id(NBT_STREAM_STD_ARGS_INNER, uint8_t tag);

  /**
   * @brief Parse the header of the tag (check type & name)
   *
   * @param strm the stream to read
   * @param N the number of bytes in the stream
   * @param tag for testing the tag
   * @return a parse result, SUCCESS if parsing finished properly, UNFINISHED if
   * parsing has not been completed due to lack of data and FAILED otherwise
   */
  ParseResult parse_type_header(NBT_STREAM_STD_ARGS_INNER, uint8_t tag);

  // ##########################################################################
  // Utils methods for parsing
  // ##########################################################################
protected:
  /**
   * @brief Modify both stream and left counter.
   * @param inc
   */
  inline void inc_stream(NBT_STREAM_STD_ARGS_INNER, size_t inc = 1) {
    *strm += inc;
    N -= inc;
  }

protected:
  char *name = nullptr;
  size_t read_bytes = 0;
  unsigned short name_length = 0;
  solis::FlagC_t parse_flags{NONE};
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

  ParseResult NBT_STREAM_PARSE_HEADER override;
};

typedef NBTPrimitive<int8_t> Byte;        //!< Byte field
typedef NBTPrimitive<uint8_t> Boolean;    //!< Boolean use a byte field
typedef NBTPrimitive<int16_t> Short;      //!< Short value field
typedef NBTPrimitive<int32_t> Int;        //!< Int value field
typedef NBTPrimitive<int64_t> Long;       //!< Long value field
typedef NBTPrimitive<float> Float;        //!< Float value field
typedef NBTPrimitive<double> Double;      //!< Double value field
typedef NBTPrimitive<std::string> String; //!< String value field

// ============================================================================
// NBT Lists implementation
// ============================================================================

/**
 * @brief NBT list base interface
 * @tparam T the type of the list elements
 */
template <typename T> struct AbstractList : NBTType, std::vector<T> {
  ParseResult NBT_STREAM_PARSE_HEADER override;
};
typedef AbstractList<NBTType *> List; //!< List of any NBT type
typedef AbstractList<Byte> ByteArray; //!< List of bytes
typedef AbstractList<Int> IntArray;   //!< List of int
typedef AbstractList<Long> LongArray; //!< List of long

// ============================================================================
// NBT Object implementation
// ============================================================================

/**
 * @brief NBT general object.
 */
struct Compound : NBTType, std::map<String, NBTType *> {
  ParseResult NBT_STREAM_PARSE_HEADER override;
};

} // namespace minecraft::nbt

#endif