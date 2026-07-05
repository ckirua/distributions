#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void rayleigh_sample_batch(
    double* out, std::size_t n, double scale, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    static constexpr std::size_t kBlock = 256;
    double buf[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(buf, chunk);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] = scale * std::sqrt(-2.0 * std::log1p(-buf[j]));
        }
        i += chunk;
    }
}

}  // namespace distributions::detail::fast
