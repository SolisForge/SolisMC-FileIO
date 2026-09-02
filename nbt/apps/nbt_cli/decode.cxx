// ============================================================================
// Project: SOLISMC_IO
//
// Bytes decoder implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "decode.hxx"
#include <concepts>
#include <cstdlib>
#include <iostream>

#include "minecraft/io/nbt/bytes/base/float.hxx"
#include "minecraft/io/nbt/bytes/base/integral.hxx"

using namespace minecraft::nbt::byte;

template <typename T>
int display_value(T const &value, ParseResult const &result) {
  if (result == ParseResult::ENDED)
    std::cout << value << std::endl;
  else
    std::cout << "UNFINISHED" << std::endl;
  return (int)(result);
}

template <std::signed_integral T> int decode_integral(Stream &strm) {
  IntParseState state;
  T value;
  auto ret = java::read_int<T>(strm, state, value);
  return display_value(value, ret);
}

template <std::floating_point T> int decode_float(Stream &strm) {
  FloatParseState state;
  T value;
  auto ret = java::read_float<T>(strm, state, value);
  return display_value(value, ret);
}

// ============================================================================
int decode_bytes(std::string &bytes, const Tags tag) {
  Stream strm{bytes.data(), bytes.length()};
  using enum minecraft::nbt::Tags;
  switch (tag) {
    // Integral type
  case BYTE:
    return decode_integral<int8_t>(strm);
  case SHORT:
    return decode_integral<int16_t>(strm);
  case INT:
    return decode_integral<int32_t>(strm);
  case LONG:
    return decode_integral<int64_t>(strm);

  // Floating point type
  case FLOAT:
    return decode_float<float>(strm);
  case DOUBLE:
    return decode_float<double>(strm);

  default:
    throw DecodingNotImplemented(tag);
  }
}
