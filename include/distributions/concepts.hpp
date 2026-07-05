#pragma once

#include "distributions/rng.hpp"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace distributions {

/// Discrete batch element types (counts, categories).
template <typename T>
struct is_discrete_sample
    : std::bool_constant<std::is_same_v<T, int> || std::is_same_v<T, std::int32_t>> {};

template <typename T>
inline constexpr bool is_discrete_sample_v = is_discrete_sample<T>::value;

/// Continuous batch element types (Phase 3 adds ``float`` in later batches).
template <typename T>
struct is_continuous_sample
    : std::bool_constant<std::is_same_v<T, float> || std::is_same_v<T, double>> {};

template <typename T>
inline constexpr bool is_continuous_sample_v = is_continuous_sample<T>::value;

template <typename T>
struct is_valid_sample : std::bool_constant<is_discrete_sample_v<T> || is_continuous_sample_v<T>> {
};

template <typename T>
inline constexpr bool is_valid_sample_v = is_valid_sample<T>::value;

/// Internal math for ``float`` output paths (mixed-precision policy; batch 3+).
template <typename Sample>
using compute_type_t = double;

/// Hand-written distribution interface (Tier A entry point; ``Sample`` from batch 2+).
template <typename Dist, typename Sample, typename RNG = Pcg32>
concept Distribution = requires(const Dist dist, Sample* out, std::size_t n, RNG& rng) {
    { dist.sample(rng) } -> std::convertible_to<Sample>;
    { dist.sample_batch(out, n, rng) } -> std::same_as<void>;
    { dist.mean() } -> std::convertible_to<double>;
    { dist.variance() } -> std::convertible_to<double>;
};

template <typename Dist, typename RNG = Pcg32>
using sample_type_t = decltype(std::declval<const Dist&>().sample(std::declval<RNG&>()));

}  // namespace distributions
