#ifndef NBT_IO_PARSER_COMPOUNDS_HPP
#define NBT_IO_PARSER_COMPOUNDS_HPP

/**
  ==================================== NBT ====================================

  This file contains the definition of the arrays compounds of NBT streams.
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
// Compound Parser
// ############################################################################

/**
 * @brief Implementation of the parser for the NBT Compound type.
 *
 */
struct NBTCompoundParser : NBTTypeParser<Compound> {};

} // namespace minecraft::nbt

#endif