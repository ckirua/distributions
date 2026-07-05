#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void weibull_sample_batch(
    double* out, std::size_t n, double shape, double scale, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    const double inv_shape = 1.0 / shape;
    static constexpr std::size_t kBlock = 256;
    double buf[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(buf, chunk);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] = scale * std::pow(-std::log1p(-buf[j]), inv_shape);
        }
        i += chunk;
    }
}

}  // namespace distributions::detail::fast
