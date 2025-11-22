// ============================================================================
// Project: SOLISMC_FILEIO
//
//
//
// Author    Meltwin (github@meltwin.fr)
// Date      20/11/2025 (created 20/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================

#include "minecraft/nbt/parser/integral.hpp"
#include "minecraft/nbt/types/integral.hpp"
#include <cstdint>
#include <iostream>

using namespace minecraft::nbt;

int main() {

  BytesParser<Tags::Short> parser;

  char *test = new char[7]{"\x08\x01\x16\x00os"};
  unsigned long N = 7;

  char *p = test;
  auto ret = parser.parse(&p, N);
  auto value = parser.get();

  parser.reset();
  ret = parser.parse(&p, N);
  auto value2 = parser.get();

  std::cout << "Ret= " << ParseResult::getName(ret) << Tags::getName(Tags::Byte)
            << " Value=" << value << " " << value2 << std::endl;
  delete[] test;
}