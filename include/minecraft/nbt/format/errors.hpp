#ifndef NBT_ERRORS_HPP
#define NBT_ERRORS_HPP

/**
  ==================================== NBT ====================================

  This file contains the definition of the errors used when working with NBT
  objects.

  @author    Meltwin
  @date      05/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "minecraft/nbt/format/tags.hpp"
#include <solis/utils/errors.hpp>

namespace minecraft::nbt {

struct MismatchNBTTagError : solis::SolisError {
  sconstchar MSG_FORMAT{
      "Mismatched NBT tags: expected {:d} ({:s}), got {:d} ({:s})"};

  MismatchNBTTagError(NBTTags got, NBTTags expected)
      : solis::SolisError(fmt::format(MSG_FORMAT, TagID(got), TagStr(got),
                                      TagID(expected), TagStr(expected))) {}
  MismatchNBTTagError(uint8_t got, uint8_t expected)
      : solis::SolisError(fmt::format(MSG_FORMAT, got, TagStr(got), expected,
                                      TagStr(expected))) {}

protected:
  std::string message;
};

} // namespace minecraft::nbt

#endif