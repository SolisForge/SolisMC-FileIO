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
#include <format>
#include <solis/utils/errors.hpp>
#include <string>

using minecraft::nbt::Tags;

// ============================================================================
struct DecodingNotImplemented : public solis::SolisError {

  explicit DecodingNotImplemented(Tags const tag)
      : solis::SolisError(std::format(
            "Decoding for type {} is not implemented ", to_str(tag))) {}
};

// ============================================================================
int decode_bytes(std::string &bytes, Tags const tag);