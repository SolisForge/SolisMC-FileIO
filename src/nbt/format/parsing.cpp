#include "minecraft/nbt/format/errors.hpp"
#include "minecraft/nbt/format/tags.hpp"
#include "minecraft/nbt/format/types.hpp"

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

using solis::has_flag;

bool NBTType::check_tag_id(NBT_STREAM_STD_ARGS_INNER, uint8_t tag) {
  // Quick return if already parsed
  if (has_flag(parse_flags, (solis::FlagC_t)ParseFlags::ID))
    return true;
  if ((*strm)[0] != tag)
    return false;

  // If true advanced pointer
  inc_stream(strm, N);
  parse_flags |= ParseFlags::ID;
  return true;
}

ParseResult NBTType::read_name_size(NBT_STREAM_STD_ARGS_INNER) {
  // Quick return if already parsed
  if (has_flag(parse_flags, (solis::FlagC_t)ParseFlags::NAME_SIZE))
    return ParseResult::SUCCESS;

  // Read the two bytes of size
  if (read_bytes == 0) {
    if (N < 1)
      return ParseResult::UNFINISHED;
    name_length = ((size_t)(*strm)[0]) << 8;
    read_bytes++;
    inc_stream(strm, N);
  }
  if (read_bytes == 1) {
    if (N < 1)
      return ParseResult::UNFINISHED;
    name_length = name_length + (size_t)(*strm)[0];
    inc_stream(strm, N);
  }
  read_bytes = 0;
  parse_flags |= ParseFlags::NAME_SIZE;
  return ParseResult::SUCCESS;
}

ParseResult NBTType::read_name(NBT_STREAM_STD_ARGS_INNER) {
  // Quick return if already parsed
  if (has_flag(parse_flags, (solis::FlagC_t)ParseFlags::NAME))
    return ParseResult::SUCCESS;

  // If name has not been allocated
  if (name == nullptr)
    name = new char[name_length];

  // Parse name bytes
  while (N > 0 && read_bytes < name_length) {
    name[read_bytes++] = (*strm)[0];
    inc_stream(strm, N);
  }
  name[read_bytes] = '\0';

  // If parsing for the name finished, set the flag and continue
  if (read_bytes == name_length) {
    parse_flags |= ParseFlags::NAME;
    read_bytes = 0;
    return ParseResult::SUCCESS;
  }
  return ParseResult::UNFINISHED;
}

ParseResult NBTType::parse_type_header(NBT_STREAM_STD_ARGS_INNER, uint8_t tag) {
  // Check if at least one byte to start the parsing
  if (N <= 0)
    return ParseResult::UNFINISHED;

  // Check for tag
  if (!check_tag_id(strm, N, tag))
    throw MismatchNBTTagError((*strm)[0], tag);

  // Parse the name field
  ParseResult ret;
  if (ret = read_name_size(strm, N); ret != ParseResult::SUCCESS)
    return ret;
  if (ret = read_name(strm, N); ret != ParseResult::SUCCESS)
    return ret;

  return ParseResult::SUCCESS;
}

// ============================================================================
// NBT primitives
// ============================================================================

template <> ParseResult Byte::NBT_STREAM_PARSE_IMPL {
  // Parse the header
  if (auto ret = parse_type_header(&strm, N, TagID<NBTTags::Byte>());
      ret != ParseResult::SUCCESS)
    return ret;

  if (N < 1)
    return ParseResult::UNFINISHED;

  val = (int8_t)(strm)[0];
  inc_stream(&strm, N);

  return ParseResult::SUCCESS;
}
template <> ParseResult Boolean::NBT_STREAM_PARSE_IMPL {
  return ParseResult::SUCCESS;
}
template <> ParseResult Short::NBT_STREAM_PARSE_IMPL {
  return ParseResult::SUCCESS;
}
template <> ParseResult Int::NBT_STREAM_PARSE_IMPL {
  return ParseResult::SUCCESS;
}
template <> ParseResult Long::NBT_STREAM_PARSE_IMPL {
  return ParseResult::SUCCESS;
}
template <> ParseResult Float::NBT_STREAM_PARSE_IMPL {
  return ParseResult::SUCCESS;
}
template <> ParseResult Double::NBT_STREAM_PARSE_IMPL {
  return ParseResult::SUCCESS;
}
template <> ParseResult String::NBT_STREAM_PARSE_IMPL {
  return ParseResult::SUCCESS;
}

// ============================================================================
// NBT Lists implementation
// ============================================================================

// ============================================================================
// NBT Object implementation
// ============================================================================

} // namespace minecraft::nbt