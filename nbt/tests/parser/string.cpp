// ============================================================================
// Project: SOLISMC-FILEIO
//
// Unittests for NBT::String byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      05/01/2026 (created 05/01/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc/tests/nbt/string.hpp"
#include "minecraft/nbt/parsers/base.hpp"
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::String>") {
  BytesParser<std::string> parser;

  SUBCASE("[STRING] Normal case") {
    auto *p = static_cast<const StreamChar *>(STRING_ONE.STREAM);
    auto n = STRING_ONE.N;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::SUCCESS);
    CHECK_EQ(parser.get().size(), STRING_RAW_LENGTH_ONE);
    CHECK_EQ(parser.get(), STRING_ONE.value);
    CHECK_EQ(n, 0);
  }
}