#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline int negative_binomial_sample(SplitMix64Stream& rng, int r, double p) {
    const std::uint64_t threshold =
        static_cast<std::uint64_t>(p * static_cast<double>(1ULL << 53));
    int failures = 0;
    int successes = 0;
    while (successes < r) {
        if ((rng.next_u64() >> 11U) < threshold) {
            ++successes;
        } else {
            ++failures;
        }
    }
    return failures;
}

inline void negative_binomial_sample_batch(
    int* out, std::size_t n, int r, double p, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = negative_binomial_sample(rng, r, p);
    }
}

}  // namespace distributions::detail::fast
