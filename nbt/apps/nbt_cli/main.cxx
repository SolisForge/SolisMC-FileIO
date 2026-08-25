// ============================================================================
// Project: SOLISMC_IO
//
// NBT cli entrypoint
// This program allow user to decode / encode NBT from a terminal.
//
// Author    Meltwin (github@meltwin.fr)
// Date      25/08/2026 (created 25/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/base/common.hpp"
#include "minecraft/io/nbt/bytes/base/integral.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <format>
#include <iostream>
#include <sstream>

using minecraft::nbt::byte::IntParseState;
using minecraft::nbt::byte::ParseResult;
using minecraft::nbt::byte::read_int;
using minecraft::nbt::byte::Stream;

// ============================================================================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  // Read byte from cin
  std::stringstream ss;
  char b;
  (void)!std::freopen(std::nullptr_t{}, "rb", stdin);
  while (std::cin >> b)
    ss << b;

  int value{0};
  IntParseState state;
  auto arg = ss.str();
  Stream p{arg.c_str(), arg.size()};

  if (auto ret = read_int<>(p, state, value); ret == ParseResult::UNFINISHED) {
    std::cout << "Parsing could not finish ... " << std::endl;
  } else {
    std::cout << "Parsed NBT char stream ";
    for (auto &c : arg) {
      std::cout << std::format("\\x{:02x}", c);
    }
    std::cout << " to " << value << std::endl;
  }

  return 0;
}