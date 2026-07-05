#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

/// Bernoulli-sum binomial for moderate trial counts (used by binomial / beta-binomial).
inline constexpr int kMaxBernoulliSumTrials = 256;

inline int binomial_bernoulli_sum(SplitMix64Stream& rng, int trials, double p) {
    const std::uint64_t threshold =
        static_cast<std::uint64_t>(p * static_cast<double>(1ULL << 53));
    int sum = 0;
    for (int t = 0; t < trials; ++t) {
        sum += (rng.next_u64() >> 11U) < threshold ? 1 : 0;
    }
    return sum;
}

inline void binomial_sample_batch(
    int* out, std::size_t n, int trials, double p, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = binomial_bernoulli_sum(rng, trials, p);
    }
}

}  // namespace distributions::detail::fast
