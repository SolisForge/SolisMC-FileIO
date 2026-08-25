// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Short byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc_io/dataset/0_short.hpp"
#include "common.hpp"
#include "minecraft/io/nbt/bytes.hpp" // IWYU pragma: keep
#include <array>
#include <cstdint>
#include <doctest/doctest.h>

using namespace minecraft::nbt;
using INT_TYPE = int16_t;
constexpr auto TYPE_SIZE{sizeof(INT_TYPE)};

// ============================================================================
TEST_CASE("BytesParser<NBT::Short>") {

  ByteParser<INT_TYPE> parser;

  //  --------------------------------------------------------------------------
  SUBCASE("[SHORT1] Normal case") {
    auto *p = SHORT1::BYTES;
    auto n = SHORT1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), SHORT1::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[SHORT2] Negative case") {
    auto *p = SHORT2::BYTES;
    auto n = SHORT2::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), SHORT2::VALUE);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[SHORT3] Big value case") {
    auto *p = SHORT3::BYTES;
    auto n = SHORT3::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), SHORT3::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_SHORT1] Two shorts") {
    auto *p = STREAM_SHORT1::STREAM;
    auto n = STREAM_SHORT1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_SHORT1::N_VALUES; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_SHORT1::VALUES[i]);
      CHECK_EQ(n, STREAM_SHORT1::N_BYTES - TYPE_SIZE * (i + 1));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_SHORT1] Incomplete stream") {
    auto *p = STREAM_SHORT1::STREAM;
    // We induce an incomplete stream
    auto n = STREAM_SHORT1::N_BYTES - 2;

    for (std::size_t i = 0; i < STREAM_SHORT1::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_SHORT1::VALUES[i]);
      CHECK_EQ(n, STREAM_SHORT1::N_BYTES - 2 - TYPE_SIZE * (i + 1));
    }

    // Incomplete parsing
    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::UNFINISHED);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
}

// ============================================================================
TEST_CASE("BytesWriter<NBT::Short>") {

  ByteWriter<INT_TYPE> dumper;
  std::array<char, TYPE_SIZE> buffer;

  //  --------------------------------------------------------------------------
  SUBCASE("[SHORT1] Normal case") {
    dumper.set(SHORT1::VALUE);
    auto p = buffer.data();
    std::size_t n = SHORT1::N_BYTES;

    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, SHORT1::BYTES, TYPE_SIZE);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[SHORT2] Negative case") {
    dumper.set(SHORT2::VALUE);
    auto p = buffer.data();
    auto n = SHORT2::N_BYTES;
    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, SHORT2::BYTES, TYPE_SIZE);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[SHORT3] Big value case") {
    dumper.set(SHORT3::VALUE);
    auto p = buffer.data();
    auto n = SHORT3::N_BYTES;
    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, SHORT3::BYTES, TYPE_SIZE);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STREAM_SHORT1] Two shorts") {
    std::array<char, STREAM_SHORT1::N_BYTES> long_buffer;
    auto *p = long_buffer.data();
    auto n = STREAM_SHORT1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_SHORT1::N_VALUES; i++) {
      dumper.set(STREAM_SHORT1::VALUES[i]);

      auto ret = dumper.write(p, n);
      CHECK_EQ(ret, WriteResult::ENDED);
      CHECK_EQ(n, STREAM_SHORT1::N_BYTES - TYPE_SIZE * (i + 1));
    }
    buffers_are_equal(long_buffer, STREAM_SHORT1::STREAM,
                      STREAM_SHORT1::N_BYTES);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STREAM_SHORT1] Incomplete stream") {
    std::array<char, STREAM_SHORT1::N_BYTES> long_buffer;
    auto *p = long_buffer.data();
    // We induce an incomplete stream
    auto n = STREAM_SHORT1::N_BYTES - 2;

    for (std::size_t i = 0; i < STREAM_SHORT1::N_VALUES - 1; i++) {
      dumper.set(STREAM_SHORT1::VALUES[i]);
      auto ret = dumper.write(p, n);
      CHECK_EQ(ret, WriteResult::ENDED);
      CHECK_EQ(n, STREAM_SHORT1::N_BYTES - 2 - TYPE_SIZE * (i + 1));
    }

    // Incomplete parsing
    dumper.set(STREAM_SHORT1::VALUES[STREAM_SHORT1::N_VALUES - 1]);
    auto ret = dumper.write(p, n);
    CHECK_EQ(ret, WriteResult::UNFINISHED);
    CHECK_EQ(n, 0);

    // Check buffers
    uint8_t max_index = STREAM_SHORT1::N_BYTES - 2;
    for (std::size_t i = 0; i < max_index; i++)
      CHECK_EQ(long_buffer[i], STREAM_SHORT1::STREAM[i]);
    for (uint8_t i = max_index; i < STREAM_SHORT1::N_VALUES * TYPE_SIZE; i++)
      CHECK_EQ(long_buffer[i], 0);
  }
}