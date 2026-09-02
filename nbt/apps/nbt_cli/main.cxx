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
#include "decode.hxx"
#include <cstdio>
#include <cstring>

#include "options.hxx"
#include "readers.hxx"

// ============================================================================
int main(int argc, char **argv) {
  auto opts = parse_args(argc, argv);

  // Read input
  auto bytes = (opts.input == ReadFrom::STDIN) ? from_stdin()
                                               : from_file(opts.input_file);
  try {
    auto error = decode_bytes(bytes, opts.data_type);
    return 0;
  } catch (...) {
    return 1;
  }
}