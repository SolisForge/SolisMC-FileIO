// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Int byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc_io/dataset/0_int.hpp"
#include "minecraft/io/nbt/bytes.hpp" // IWYU pragma: keep
#include <array>
#include <cstddef>
#include <cstdint>
#include <doctest/doctest.h>

#include "common.hpp"

using namespace minecraft::nbt;

using INT_TYPE = int32_t;
constexpr auto TYPE_SIZE{sizeof(INT_TYPE)};

// ============================================================================
TEST_CASE("BytesParser<NBT::Int>") {

  ByteParser<INT_TYPE> parser;

  //  --------------------------------------------------------------------------
  SUBCASE("[INT1] Normal case") {
    auto *p = INT1::BYTES;
    auto n = INT1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), INT1::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[INT2] Negative case") {
    auto *p = INT2::BYTES;
    auto n = INT2::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), INT2::VALUE);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[INT3] Big value case") {
    auto *p = INT3::BYTES;
    auto n = INT3::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), INT3::VALUE);
    CHECK_EQ(parser.get(), INT3::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_INT1] Two ints") {
    auto *p = STREAM_INT1::STREAM;
    auto n = STREAM_INT1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_INT1::N_VALUES; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_INT1::VALUES[i]);
      CHECK_EQ(n, STREAM_INT1::N_BYTES - TYPE_SIZE * (i + 1));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_INT1] Incomplete stream") {
    auto *p = STREAM_INT1::STREAM;
    // We induce an incomplete stream
    auto n = STREAM_INT1::N_BYTES - 2;

    for (std::size_t i = 0; i < STREAM_INT1::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_INT1::VALUES[i]);
      CHECK_EQ(n, STREAM_INT1::N_BYTES - 2 - TYPE_SIZE * (i + 1));
    }

    // Incomplete parsing
    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::UNFINISHED);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
}

// ============================================================================
TEST_CASE("BytesWriter<NBT::Int>") {

  ByteWriter<INT_TYPE> dumper;
  std::array<char, TYPE_SIZE> buffer;

  //  --------------------------------------------------------------------------
  SUBCASE("[INT1] Normal case") {
    dumper.set(INT1::VALUE);
    auto p = buffer.data();
    std::size_t n = INT1::N_BYTES;

    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, INT1::BYTES, TYPE_SIZE);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[INT2] Negative case") {
    dumper.set(INT2::VALUE);
    auto p = buffer.data();
    auto n = INT2::N_BYTES;
    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, INT2::BYTES, TYPE_SIZE);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[INT3] Big value case") {
    dumper.set(INT3::VALUE);
    auto p = buffer.data();
    auto n = INT3::N_BYTES;
    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, INT3::BYTES, TYPE_SIZE);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STREAM_INT1] Two ints") {
    std::array<char, STREAM_INT1::N_BYTES> long_buffer;
    auto *p = long_buffer.data();
    auto n = STREAM_INT1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_INT1::N_VALUES; i++) {
      dumper.set(STREAM_INT1::VALUES[i]);

      auto ret = dumper.write(p, n);
      CHECK_EQ(ret, WriteResult::ENDED);
      CHECK_EQ(n, STREAM_INT1::N_BYTES - TYPE_SIZE * (i + 1));
    }
    buffers_are_equal(long_buffer, STREAM_INT1::STREAM, STREAM_INT1::N_BYTES);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STREAM_INT1] Incomplete stream") {
    std::array<char, STREAM_INT1::N_BYTES> long_buffer;
    auto *p = long_buffer.data();
    // We induce an incomplete stream
    auto n = STREAM_INT1::N_BYTES - 2;

    for (std::size_t i = 0; i < STREAM_INT1::N_VALUES - 1; i++) {
      dumper.set(STREAM_INT1::VALUES[i]);
      auto ret = dumper.write(p, n);
      CHECK_EQ(ret, WriteResult::ENDED);
      CHECK_EQ(n, STREAM_INT1::N_BYTES - 2 - TYPE_SIZE * (i + 1));
    }

    // Incomplete parsing
    dumper.set(STREAM_INT1::VALUES[STREAM_INT1::N_VALUES - 1]);
    auto ret = dumper.write(p, n);
    CHECK_EQ(ret, WriteResult::UNFINISHED);
    CHECK_EQ(n, 0);

    // Check buffers
    uint8_t max_index = STREAM_INT1::N_BYTES - 2;
    for (std::size_t i = 0; i < max_index; i++)
      CHECK_EQ(long_buffer[i], STREAM_INT1::STREAM[i]);
    for (uint8_t i = max_index; i < STREAM_INT1::N_VALUES * TYPE_SIZE; i++)
      CHECK_EQ(long_buffer[i], 0);
  }
}