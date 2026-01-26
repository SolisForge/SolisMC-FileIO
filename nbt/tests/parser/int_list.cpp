// ============================================================================
// Project: SOLISMC-FILEIO
//
// Unittests for NBT::IntArray byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      05/01/2026 (created 05/01/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parser.hpp" // IWYU pragma: keep
#include "solismc_dataset/nbt/2_int_array.hpp"
#include <cstdint>
#include <doctest/doctest.h>

using namespace minecraft::nbt;

template <typename T>
constexpr std::size_t VECTOR_STRIDE(std::size_t n_values) {
  return sizeof(int32_t) + n_values * sizeof(T);
}

// ============================================================================
TEST_CASE("BytesParser<NBT::IntArray>") {
  BytesParser<std::vector<std::int32_t>> parser;

  SUBCASE("[INT_VEC1] Normal case") {
    auto *p = static_cast<const StreamChar *>(INT_VEC1::STREAM);
    auto n = INT_VEC1::LENGTH;
    auto ret = parser.parse(p, n);

    // Successful parse
    CHECK_EQ(ret, ParseResult::SUCCESS);

    // Same list content
    auto vector = parser.get();
    CHECK_EQ(parser.get_length(), INT_VEC1::VALUES_LENGTH[0]);
    CHECK_EQ(vector->size(), INT_VEC1::VALUES_LENGTH[0]);
    for (std::size_t i = 0; i < vector->size(); i++) {
      CHECK_EQ((*vector)[i], INT_VEC1::VALUES[0][i]);
    }

    // Check no byte remaining
    CHECK_EQ(n, 0);
  }
  SUBCASE("[INT_VEC2] Normal case") {
    auto *p = static_cast<const StreamChar *>(INT_VEC2::STREAM);
    auto n = INT_VEC2::LENGTH;
    auto ret = parser.parse(p, n);

    // Successful parse
    CHECK_EQ(ret, ParseResult::SUCCESS);

    // Same list content
    auto vector = parser.get();
    CHECK_EQ(vector->size(), INT_VEC2::VALUES_LENGTH[0]);
    for (std::size_t i = 0; i < vector->size(); i++) {
      CHECK_EQ((*vector)[i], INT_VEC2::VALUES[0][i]);
    }

    // Check no byte remaining
    CHECK_EQ(n, 0);
  }

  SUBCASE("[MULTIPLE_INT_VECS] Multiple int array") {
    auto *p = static_cast<const StreamChar *>(MULTIPLE_INT_VECS::STREAM);
    auto n = MULTIPLE_INT_VECS::LENGTH;
    auto REMAINING = MULTIPLE_INT_VECS::LENGTH;

    // Check for all values
    for (std::size_t value_idx = 0; value_idx < MULTIPLE_INT_VECS::N_VALUES;
         value_idx++) {

      // Parse new value
      auto ret = parser.parse(p, n);
      REMAINING -=
          VECTOR_STRIDE<int32_t>(MULTIPLE_INT_VECS::VALUES_LENGTH[value_idx]);

      // Successful parse
      CHECK_EQ(ret, ParseResult::SUCCESS);

      // Same list content
      auto vector = parser.get();
      CHECK_EQ(vector->size(), MULTIPLE_INT_VECS::VALUES_LENGTH[value_idx]);
      for (std::size_t i = 0; i < vector->size(); i++) {
        CHECK_EQ((*vector)[i], MULTIPLE_INT_VECS::VALUES[value_idx][i]);
      }

      CHECK_EQ(n, REMAINING);
    }

    // Check no byte remaining
    CHECK_EQ(n, 0);
  }

  SUBCASE("[INCOMPLETE_INT_VECS] Incomplete multiple int array") {
    auto *p = static_cast<const StreamChar *>(INCOMPLETE_INT_VECS::STREAM);
    auto n = INCOMPLETE_INT_VECS::LENGTH;
    auto REMAINING = INCOMPLETE_INT_VECS::LENGTH;

    // Check for all values
    for (std::size_t value_idx = 0;
         value_idx < INCOMPLETE_INT_VECS::N_VALUES - 1; value_idx++) {

      // Parse new value
      auto ret = parser.parse(p, n);
      REMAINING -=
          VECTOR_STRIDE<int32_t>(INCOMPLETE_INT_VECS::VALUES_LENGTH[value_idx]);

      // Successful parse
      CHECK_EQ(ret, ParseResult::SUCCESS);

      // Same list content
      auto vector = parser.get();
      CHECK_EQ(vector->size(), INCOMPLETE_INT_VECS::VALUES_LENGTH[value_idx]);
      for (std::size_t i = 0; i < vector->size(); i++) {
        CHECK_EQ((*vector)[i], INCOMPLETE_INT_VECS::VALUES[value_idx][i]);
      }

      CHECK_EQ(n, REMAINING);
    }

    auto ret = parser.parse(p, n);

    // Unfinished parse
    CHECK_EQ(ret, ParseResult::UNFINISHED);

    CHECK_FALSE(parser.is_parsed());
    CHECK_EQ(parser.get_length(), 0);
    CHECK_EQ(parser.get(), nullptr);

    // Check no byte remaining
    CHECK_EQ(n, 0);
  }
}