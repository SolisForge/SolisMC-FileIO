// ============================================================================
// Project: SOLISMC_IO
//
// Bytes decoder
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once
#include "minecraft/io/nbt/tags.hxx"
#include <string>

using minecraft::nbt::Tags;

int decode_bytes(std::string const &bytes, Tags const tag);