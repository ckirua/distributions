#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void exponential_sample_batch(
    double* out, std::size_t n, double rate, std::uint64_t seed) {
    const double inv_rate = -1.0 / rate;
    SplitMix64Stream rng;
    rng.seed(seed);
    constexpr std::size_t kBlock = 256;
    double uniforms[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(uniforms, chunk);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] = std::log1p(-uniforms[j]) * inv_rate;
        }
        i += chunk;
    }
}

/// Tier B for ``float`` output (compute in ``double``, store as ``float``).
inline void exponential_sample_batch(
    float* out, std::size_t n, double rate, std::uint64_t seed) {
    const double inv_rate = -1.0 / rate;
    SplitMix64Stream rng;
    rng.seed(seed);
    constexpr std::size_t kBlock = 256;
    double uniforms[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(uniforms, chunk);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] = static_cast<float>(std::log1p(-uniforms[j]) * inv_rate);
        }
        i += chunk;
    }
}

}  // namespace distributions::detail::fast
