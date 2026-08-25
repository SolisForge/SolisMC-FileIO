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
#include "solismc_io/dataset/1_float_list.hpp"
#include <algorithm>
#include <doctest/doctest.h>

using namespace minecraft::nbt;

// ============================================================================
TEST_CASE("BytesParser<NBT::Array[Float]>") {

  ByteParser<std::vector<float>> parser;

  //  --------------------------------------------------------------------------
  SUBCASE("[FLOATLIST1] Normal case") {
    auto *p = FLOATLIST1::BYTES;
    auto n = FLOATLIST1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK(parser.is_done());
    CHECK_EQ(n, 0);
    auto v = parser.get();
    CHECK_EQ(v.size(), FLOATLIST1::N_VALUES);
    for (size_t i = 0; i < std::min(FLOATLIST1::N_VALUES, v.size()); i++) {
      CHECK_EQ(v[i], FLOATLIST1::VALUE[i]);
    }
  }
}