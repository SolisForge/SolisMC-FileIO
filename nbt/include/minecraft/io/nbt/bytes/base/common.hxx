// ============================================================================
// Project: SOLISMC-IO
//
// Common resources for byte IO
//
// Author    Meltwin (github@meltwin.fr)
// Date      10/08/2026 (created 10/08/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#pragma once

#include <cstddef>
#include <cstdint>

namespace minecraft::nbt::byte {

// ============================================================================

/**
 * @brief Result of the byte parsing
 */
enum class ParseResult : uint8_t {
  UNFINISHED, //< Parsing is not finished yet
  ENDED       //< Parsing has ended
};

// ============================================================================

/**
 * @brief Stream container
 */
struct Stream {
  const char *data = std::nullptr_t{};
  std::size_t n{0};

  /**
   * @brief Increase the stream iterator
   */
  inline void inc(std::size_t const i = 1) {
    if (n < i)
      return;
    data += i;
    n -= i;
  }
};

} // namespace minecraft::nbt::byte