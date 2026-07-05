#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void pareto_sample_batch(
    double* out, std::size_t n, double alpha, double scale, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    const double inv_alpha = 1.0 / alpha;
    static constexpr std::size_t kBlock = 256;
    double buf[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(buf, chunk);
        for (std::size_t j = 0; j < chunk; ++j) {
            const double u = std::max(buf[j], 1e-300);
            out[i + j] = scale / std::pow(u, inv_alpha);
        }
        i += chunk;
    }
}

}  // namespace distributions::detail::fast
