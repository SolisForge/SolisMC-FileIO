// ============================================================================
// Project: SOLISMC_FILEIO
//
// Unittests for NBT::Byte byte parsing.
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "solismc_io/dataset/0_byte.hpp"
#include "common.hpp"
#include "minecraft/io/nbt/bytes.hpp" // IWYU pragma: keep
#include <array>
#include <cstdint>
#include <doctest/doctest.h>

using namespace minecraft::nbt;
using INT_TYPE = int8_t;
constexpr std::size_t TYPE_SIZE{sizeof(INT_TYPE)};
constexpr auto INCOMPLETE_OFFSET{1};

// ============================================================================
TEST_CASE("BytesParser<NBT::Byte>") {

  ByteParser<INT_TYPE> parser;

  //  --------------------------------------------------------------------------
  SUBCASE("[BYTE1] Normal case") {
    auto *p = BYTE1::BYTES;
    auto n = BYTE1::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), BYTE1::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[BYTE2] Negative case") {
    auto *p = BYTE2::BYTES;
    auto n = BYTE2::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), BYTE2::VALUE);
    CHECK_LT(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[BYTE3] Big value case") {
    auto *p = BYTE3::BYTES;
    auto n = BYTE3::N_BYTES;
    auto ret = parser.parse(p, n);

    CHECK_EQ(ret, ParseResult::ENDED);
    CHECK_EQ(parser.get(), BYTE3::VALUE);
    CHECK_EQ(parser.get(), BYTE3::VALUE);
    CHECK_EQ(n, 0);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_BYTE1] Two bytes") {
    auto *p = STREAM_BYTE1::STREAM;
    auto n = STREAM_BYTE1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_BYTE1::N_VALUES; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_BYTE1::VALUES[i]);
      CHECK_EQ(n, STREAM_BYTE1::N_BYTES - TYPE_SIZE * (i + 1));
    }
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[STREAM_BYTE1] Incomplete stream") {
    auto *p = STREAM_BYTE1::STREAM;
    // We induce an incomplete stream
    const auto STRM_LEN = STREAM_BYTE1::N_BYTES - INCOMPLETE_OFFSET;
    auto n = STRM_LEN;

    for (std::size_t i = 0; i < STREAM_BYTE1::N_VALUES - 1; i++) {
      auto ret = parser.parse(p, n);
      CHECK_EQ(ret, ParseResult::ENDED);
      CHECK_EQ(parser.get(), STREAM_BYTE1::VALUES[i]);
      CHECK_EQ(n, STRM_LEN - TYPE_SIZE * (i + 1));
    }

    // Incomplete parsing
    auto ret = parser.parse(p, n);
    CHECK_EQ(ret, ParseResult::UNFINISHED);
    CHECK_EQ(parser.get(), 0);
    CHECK_EQ(n, 0);
  }
}

// ============================================================================
TEST_CASE("BytesWriter<NBT::Byte>") {

  ByteWriter<INT_TYPE> dumper;
  std::array<char, TYPE_SIZE> buffer;

  //  --------------------------------------------------------------------------
  SUBCASE("[BYTE1] Normal case") {
    dumper.set(BYTE1::VALUE);
    auto p = buffer.data();
    std::size_t n = BYTE1::N_BYTES;

    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, BYTE1::BYTES, TYPE_SIZE);
  }
  //  --------------------------------------------------------------------------
  SUBCASE("[BYTE2] Negative case") {
    dumper.set(BYTE2::VALUE);
    auto p = buffer.data();
    auto n = BYTE2::N_BYTES;
    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, BYTE2::BYTES, TYPE_SIZE);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[BYTE3] Big value case") {
    dumper.set(BYTE3::VALUE);
    auto p = buffer.data();
    auto n = BYTE3::N_BYTES;
    auto ret = dumper.write(p, n);
    check_writing_ended(ret, n);
    buffers_are_equal(buffer, BYTE3::BYTES, TYPE_SIZE);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STREAM_BYTE1] Two bytes") {
    std::array<char, STREAM_BYTE1::N_BYTES> long_buffer;
    auto *p = long_buffer.data();
    auto n = STREAM_BYTE1::N_BYTES;

    for (std::size_t i = 0; i < STREAM_BYTE1::N_VALUES; i++) {
      dumper.set(STREAM_BYTE1::VALUES[i]);

      auto ret = dumper.write(p, n);
      CHECK_EQ(ret, WriteResult::ENDED);
      CHECK_EQ(n, STREAM_BYTE1::N_BYTES - TYPE_SIZE * (i + 1));
    }
    buffers_are_equal(long_buffer, STREAM_BYTE1::STREAM, STREAM_BYTE1::N_BYTES);
  }
  // --------------------------------------------------------------------------
  SUBCASE("[STREAM_BYTE1] Incomplete stream") {
    std::array<char, STREAM_BYTE1::N_BYTES> long_buffer;
    auto *p = long_buffer.data();
    // We induce an incomplete stream
    const auto STRM_LEN = STREAM_BYTE1::N_BYTES - INCOMPLETE_OFFSET;
    auto n = STRM_LEN;

    for (std::size_t i = 0; i < STREAM_BYTE1::N_VALUES - 1; i++) {
      dumper.set(STREAM_BYTE1::VALUES[i]);
      auto ret = dumper.write(p, n);
      CHECK_EQ(ret, WriteResult::ENDED);
      CHECK_EQ(n, STRM_LEN - TYPE_SIZE * (i + 1));
    }

    // Incomplete parsing
    dumper.set(STREAM_BYTE1::VALUES[STREAM_BYTE1::N_VALUES - 1]);
    auto ret = dumper.write(p, n);
    CHECK_EQ(ret, WriteResult::UNFINISHED);
    CHECK_EQ(n, 0);

    // Check buffers
    for (std::size_t i = 0; i < STRM_LEN; i++)
      CHECK_EQ(long_buffer[i], STREAM_BYTE1::STREAM[i]);
    for (uint8_t i = STRM_LEN; i < STREAM_BYTE1::N_VALUES * TYPE_SIZE; i++)
      CHECK_EQ(long_buffer[i], 0);
  }
}