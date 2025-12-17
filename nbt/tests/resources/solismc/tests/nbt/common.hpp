// ============================================================================
// Project: SOLISMC_FILEIO
//
// Common structures to make test resources for NBT.
//
// Author    Meltwin (github@meltwin.fr)
// Date      26/11/2025 (created 26/11/2025)
// Version   1.0.0
// Copyright Solis Forge | 2025
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#ifndef SOLIS_MC_NBT_TEST_COMMON_HPP
#define SOLIS_MC_NBT_TEST_COMMON_HPP

#include "minecraft/nbt/parsers/base.hpp"
#include <cstdint>
#include <minecraft/nbt/parser.hpp>

namespace solis {

using minecraft::nbt::StreamChar;

// ============================================================================

/**
 * @brief Structure to represent the NBT values as bytes
 *
 * @tparam T the output typename
 * @tparam Bytes the bytes of the value
 */
template <typename T, StreamChar... Bytes> struct NBTValue {
  using TYPE = T;

  static constexpr std::size_t N = sizeof...(Bytes);
  static_assert(
      N == sizeof(T),
      "The number of provided bytes should be equal to the output type size");

  static constexpr StreamChar STREAM[N]{Bytes...};

  explicit constexpr NBTValue<T, Bytes...>(T value) : value(value) {}

  T value;
};

template <typename Tout, typename Tin, uint8_t... B1, uint8_t... B2,
          template <typename, uint8_t...> typename C>
constexpr C<Tout, B1..., B2...> Combined(C<Tin, B1...>, C<Tin, B2...>) {
  static_assert(sizeof(Tout) == 2 * sizeof(Tin),
                "Output type should be two times the size of the input ones");
  return C<Tout, B1..., B2...>();
}

// ============================================================================

/**
 * @brief Structure to store an array of bytes to use parse
 */
template <unsigned int N = 1, StreamChar... Bytes> struct NBTstream {
  static constexpr std::size_t N_VALUES{N};
  static constexpr std::size_t STREAM_LENGTH = sizeof...(Bytes);

  static constexpr StreamChar STREAM[STREAM_LENGTH]{Bytes...};

  explicit constexpr NBTstream<N, Bytes...>() = default;

  /**
   * @brief Stream constructor from a NBTValue object
   */
  template <typename T>
  explicit constexpr NBTstream<N, Bytes...>(NBTValue<T, Bytes...>) {}

  /**
   * @brief Add a NBTValue to the stream
   */
  template <typename T, uint8_t... ValueBytes>
  constexpr NBTstream<N + 1, Bytes..., ValueBytes...>
  operator+(const NBTValue<T, ValueBytes...> &) const {
    return NBTstream<N + 1, Bytes..., ValueBytes...>();
  }
};

} // namespace solis

#endif