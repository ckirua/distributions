#pragma once

#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

/// Tier-B uniform fill on [0, 1) using `CounterRng` (single-thread).
inline void fill_uniform01(double* out, std::size_t n, std::uint64_t seed, std::uint32_t stream = 0) {
    CounterRng rng;
    rng.seed(seed, stream);
    rng.fill_uniform01(out, n);
}

/// Tier-B bounded uniform on [lo, hi) via block SplitMix fill.
inline void bounded_uniform_sample_batch(
    double* out, std::size_t n, double lo, double hi, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    const double span = hi - lo;
    constexpr std::size_t kBlock = 256;
    double buf[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(buf, chunk);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] = lo + span * buf[j];
        }
        i += chunk;
    }
}

}  // namespace distributions::detail::fast
