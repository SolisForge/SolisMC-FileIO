// ============================================================================
// Project: SOLISMC_IO
//
// File reading implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "readers.hxx"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// ============================================================================
std::string from_file(std::filesystem::path const &path) {
  if (!std::filesystem::exists((path)))
    throw std::runtime_error{
        std::format("File {} could not be found", path.c_str())};

  std::cerr << "Reading from " << path << std::endl;
  std::stringstream ss;
  std::ifstream handle;
  handle.open(path.c_str(), std::ios::binary | std::ios::in);

  // Read all
  std::string line;
  while (std::getline(handle, line))
    ss << line;

  return ss.str();
}

// ============================================================================
std::string from_stdin() {
  std::stringstream ss;
  char b;

  std::cerr << "Reading from stdin" << std::endl;
  (void)!std::freopen(std::nullptr_t{}, "rb", stdin);
  while (std::cin >> b)
    ss << b;

  return ss.str();
}