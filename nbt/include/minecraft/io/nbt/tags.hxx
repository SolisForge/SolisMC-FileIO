// ============================================================================
// Project: SOLISMC_IO
//
// NTB data type tags definition
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once

#include <cstdint>

namespace minecraft::nbt {

enum class Tags : uint8_t {
  END = 0,
  BYTE,       //< 8 bits signed integer
  SHORT,      //< 16 bits signed integer
  INT,        //< 32 bits signed integer
  LONG,       //< 64 bits signed integer
  FLOAT,      //< 32 bits signed floating point (IEEE 754-2008, binary32)
  DOUBLE,     //< 64 bits signed floating point (IEEE 754-2008, binary64)
  BYTE_ARRAY, // unsigned INT size + size BYTE
  STRING,     // unsigned SHORT size + size UTF-8 BYTEs (not null terminated)
  LIST,       // TAG + unsigned INT size + size items
  COMPOUND,   // [TAG + unsigned SHORT size + STRING[size] + payload] + TAG.END
  INT_ARRAY,  // unsigned INT size + size INT
  LONG_ARRAY, // unsigned INT size + size LONG
};

const char *to_str(Tags const tag);

} // namespace minecraft::nbt