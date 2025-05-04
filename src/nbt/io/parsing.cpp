#include "minecraft/nbt/format/errors.hpp"
#include "minecraft/nbt/io/parser_array.hpp"
#include "minecraft/nbt/io/parser_compound.hpp"
#include "minecraft/nbt/io/parser_interface.hpp"
#include "minecraft/nbt/io/parser_primitives.hpp"

#include <iostream>

/**
  ==================================== NBT ====================================

  This file contains the implementation of tehe types parsing.
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

namespace minecraft::nbt {

// ============================================================================
// Parser interface general methods
// ============================================================================

bool _INBTTypeParser::check_tag_id(NBT_STREAM_STD_ARGS, NBTTags::Tags tag) {
  // Quick return if already parsed
  if (has_flag(flags, ParseFlags::ID))
    return true;

  // Check for no head
  if (include_head) {
    if ((*strm)[0] != tag)
      return false;
    inc_stream(strm, N);
  }
  flags |= ParseFlags::ID;
  return true;
}

ParseResult _INBTTypeParser::read_name_size(NBT_STREAM_STD_ARGS,
                                            NBTType *dest) {
  // Quick return if already parsed
  if (has_flag(flags, ParseFlags::NAME_SIZE))
    return ParseResult::SUCCESS;

  // Read the two bytes of name size
  if (auto ret = read(strm, N, dest->name_length); ret != ParseResult::SUCCESS)
    return ret;
  flags |= ParseFlags::NAME_SIZE;
  return ParseResult::SUCCESS;
}

ParseResult _INBTTypeParser::read_name(NBT_STREAM_STD_ARGS, NBTType *dest) {
  // Quick return if already parsed
  if (has_flag(flags, ParseFlags::NAME))
    return ParseResult::SUCCESS;

  // Parse name bytes
  if (auto ret = read(strm, N, &dest->name, dest->name_length);
      ret != ParseResult::SUCCESS)
    return ret;
  flags |= ParseFlags::NAME;
  return ParseResult::SUCCESS;
}

ParseResult _INBTTypeParser::parse_type_header(NBT_STREAM_STD_ARGS,
                                               NBTTags::Tags tag,
                                               NBTType *dest) {
  // Check if at least one byte to start the parsing
  if (N <= 0)
    return ParseResult::UNFINISHED;

  // Check for tag
  if (!check_tag_id(NBT_STREAM_FWD_ARGS, tag))
    throw MismatchNBTTagError((*strm)[0], tag);

  // Parse the name field
  ParseResult ret;
  if (ret = read_name_size(NBT_STREAM_FWD_ARGS, dest);
      ret != ParseResult::SUCCESS)
    return ret;

  if (ret = read_name(NBT_STREAM_FWD_ARGS, dest); ret != ParseResult::SUCCESS) {
    dest->name[read_bytes] = '\0';
    return ret;
  }

  return ParseResult::SUCCESS;
}

// ============================================================================
// NBT primitives
// ============================================================================

/**
 * @brief Parse a byte value from a NBT stream
 */
template <> ParseResult NBTByteParser::parse_payload(_NBT_STREAM_STRM_ARGS) {
  if (N < 1)
    return ParseResult::UNFINISHED;

  // Parse byte value
  curr->val = (int8_t)(*strm)[0];
  inc_stream(strm, N);
  flags |= ParseFlags::PAYLOAD | ParseFlags::PAYLOAD_SIZE;
  return ParseResult::SUCCESS;
}
// ----------------------------------------------------------------------------
/**
 * @brief Parse a boolean value from a NBT stream
 */
template <> ParseResult NBTBooleanParser::parse_payload(_NBT_STREAM_STRM_ARGS) {
  if (N < 1)
    return ParseResult::UNFINISHED;

  // Parse byte value
  curr->val = (bool)(*strm)[0];
  inc_stream(strm, N);
  flags |= ParseFlags::PAYLOAD | ParseFlags::PAYLOAD_SIZE;
  return ParseResult::SUCCESS;
}
// ----------------------------------------------------------------------------
/**
 * @brief Parse short value (2 bytes) from a NBT stream
 */
template <> ParseResult NBTShortParser::parse_payload(_NBT_STREAM_STRM_ARGS) {
  if (auto ret = read(strm, N, curr->val); ret != ParseResult::SUCCESS)
    return ret;
  flags |= ParseFlags::PAYLOAD | ParseFlags::PAYLOAD_SIZE;
  return ParseResult::SUCCESS;
}
// ----------------------------------------------------------------------------
/**
 * @brief Parse an int value (4 bytes) from a NBT stream
 */
template <> ParseResult NBTIntParser::parse_payload(_NBT_STREAM_STRM_ARGS) {
  if (auto ret = read(strm, N, curr->val); ret != ParseResult::SUCCESS)
    return ret;
  flags |= ParseFlags::PAYLOAD | ParseFlags::PAYLOAD_SIZE;
  return ParseResult::SUCCESS;
}
// ----------------------------------------------------------------------------
/**
 * @brief Parse an long value (8 bytes) from a NBT stream
 */
template <> ParseResult NBTLongParser::parse_payload(_NBT_STREAM_STRM_ARGS) {
  if (auto ret = read(strm, N, curr->val); ret != ParseResult::SUCCESS)
    return ret;
  flags |= ParseFlags::PAYLOAD | ParseFlags::PAYLOAD_SIZE;
  return ParseResult::SUCCESS;
}
// ----------------------------------------------------------------------------
/**
 * @brief Parse an string value from a NBT stream
 */
ParseResult NBTStringParser::parse_payload(_NBT_STREAM_STRM_ARGS) {
  // Parse string size
  ParseResult ret;
  if (!has_flag(flags, ParseFlags::PAYLOAD_SIZE)) {
    if (ret = read(strm, N, curr->val_size); ret != ParseResult::SUCCESS)
      return ret;
    flags |= ParseFlags::PAYLOAD_SIZE;
  }

  // Parse string
  if (ret = read(strm, N, &curr->val, curr->val_size);
      ret != ParseResult::SUCCESS) {
    curr->val[read_bytes] = '\0';
    return ret;
  }
  flags |= ParseFlags::PAYLOAD;
  return ParseResult::SUCCESS;
}

// ============================================================================
// NBT Lists implementation
// ============================================================================

/**
 * @brief Parse an byte for the byte array
 */
template <> ParseResult NBTByteArrayParser::parse_item(_NBT_STREAM_STRM_ARGS) {
  if (auto ret = read(strm, N, parsing_val); ret != ParseResult::SUCCESS)
    return ret;
  return ParseResult::SUCCESS;
}

/**
 * @brief Parse an int for the int array
 */
template <> ParseResult NBTIntArrayParser::parse_item(_NBT_STREAM_STRM_ARGS) {
  if (auto ret = read(strm, N, parsing_val); ret != ParseResult::SUCCESS)
    return ret;
  return ParseResult::SUCCESS;
}

/**
 * @brief Parse an long for the long array
 */
template <> ParseResult NBTLongArrayParser::parse_item(_NBT_STREAM_STRM_ARGS) {
  if (auto ret = read(strm, N, parsing_val); ret != ParseResult::SUCCESS)
    return ret;
  return ParseResult::SUCCESS;
}

// template <> ParseResult NBTListParser::parse_item(_NBT_STREAM_STRM_ARGS) {

//   return ParseResult::SUCCESS;
// }

// ============================================================================
// NBT Object implementation
// ============================================================================

} // namespace minecraft::nbt