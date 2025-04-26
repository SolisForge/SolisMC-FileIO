#ifndef NBT_IO_PARSER_PRIMITIVES_HPP
#define NBT_IO_PARSER_PRIMITIVES_HPP

/**
  ==================================== NBT ====================================

  This file contains the definition of the primitives parsing of NBT streams.
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

// ----------------------------- Primitives Parsers ---------------------------

template <typename T, NBTTags::Tags E = Tag<T>()>
struct NBTIntegralParser : public NBTTypeParser<T, E> {
  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS) override;
  T *initialize_var() override { return new T(static_cast<T>(0)); }
};

typedef NBTIntegralParser<Byte> NBTByteParser;
typedef NBTIntegralParser<Boolean, NBTTags::Byte> NBTBooleanParser;
typedef NBTIntegralParser<Short> NBTShortParser;
typedef NBTIntegralParser<Int> NBTIntParser;
typedef NBTIntegralParser<Long> NBTLongParser;
typedef NBTIntegralParser<String> NBTStringParser;

// -------------------------- Float / Double Parsers --------------------------

/**
 * @brief Parser specialization for binary floating points.
 *
 * @tparam T the NBT type
 * @tparam B the buffer type
 */
template <typename T, typename B, size_t NB = sizeof(B)>
struct NBTBinaryFloatParser : public NBTTypeParser<T> {

  virtual ParseResult parse_payload(_NBT_STREAM_STRM_ARGS) override {
    if (auto ret = read_floating(strm, N, this->curr->val, _bytes);
        ret != ParseResult::SUCCESS)
      return ret;
    return ParseResult::SUCCESS;
  }

  T *initialize_var() override { return new T(0.0); }

protected:
  B _bytes;
};

typedef NBTBinaryFloatParser<float, uint32_t> NBTFloatParser;
typedef NBTBinaryFloatParser<double, uint32_t> NBTDoubleParser;

} // namespace minecraft::nbt

#endif