// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::ByteArray byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/io/nbt/bytes.hpp" // IWYU pragma: keep
#include "solismc_io/dataset/1_byte_list.hpp"
#include <algorithm>
#include <cstdint>
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::ByteArray>") {

  ByteParser<std::vector<int8_t>> parser;

  //  --------------------------------------------------------------------------
  SUBCASE("[BYTELIST1] Normal case") {
    auto *p = BYTELIST1::BYTES;
    auto n = BYTELIST1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK(parser.is_done());
    CHECK_EQ(n, 0);
    auto v = parser.get();
    CHECK_EQ(v.size(), BYTELIST1::N_VALUES);
    for (size_t i = 0; i < std::min(BYTELIST1::N_VALUES, v.size()); i++) {
      CHECK_EQ(v[i], BYTELIST1::VALUE[i]);
    }
  }
}