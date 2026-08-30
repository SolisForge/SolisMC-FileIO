// ============================================================================
// Project: SOLISMC_IO
//
// Option parsing implementation
//
// Author    Meltwin (github@meltwin.fr)
// Date      30/08/2026 (created 30/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "options.hxx"
#include <cstring>
#include <filesystem>
#include <stdexcept>

// ============================================================================
Tags parse_type(std::string_view const &qualifier) {
  // Integral types
  if (qualifier.compare("int8") == 0)
    return Tags::BYTE;
  else if (qualifier.compare("int16") == 0)
    return Tags::SHORT;
  else if (qualifier.compare("int32") == 0)
    return Tags::INT;
  else if (qualifier.compare("int64") == 0)
    return Tags::LONG;
  // Float types
  else if (qualifier.compare("float") == 0)
    return Tags::FLOAT;
  else if (qualifier.compare("double") == 0)
    return Tags::DOUBLE;

  return Tags::COMPOUND;
}

// ============================================================================
Options parse_args(int argc, char **argv) {
  Options opt{};

  // Skip first argument == program name
  argc--;
  argv++;

  // Parse all arguments
  while (argc > 0) {
    if (auto len = std::strlen(argv[0]); len < 2)
      throw std::invalid_argument{"Argument should have a length of 2"};

    // File option
    if (std::strncmp(argv[0], "-f", 2) == 0) {
      if (argc == 1)
        throw std::invalid_argument{"Missing path to -f"};
      opt.input = ReadFrom::FILE;
      opt.input_file = std::filesystem::path{argv[1]};
      argc -= 2;
      argv += 2;
    }
    // Type option
    else if (std::strncmp(argv[0], "-t", 2) == 0) {
      if (argc == 1)
        throw std::invalid_argument{"Missing type info to -t"};
      opt.data_type = parse_type(argv[1]);
      argc -= 2;
      argv += 2;
    }
  }

  return opt;
}