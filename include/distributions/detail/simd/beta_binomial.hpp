#pragma once

#include "distributions/detail/fast/beta_binomial.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/simd/binomial.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/config.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

inline int beta_binomial_sample_lane(
    SplitMix64Stream& sm, int trials, double alpha, double beta) {
    Pcg32 beta_rng(sm.next_u64());
    const double p = detail::sample_beta(beta_rng, alpha, beta);
    const std::uint64_t threshold =
        static_cast<std::uint64_t>(p * static_cast<double>(1ULL << 53));
#if defined(DISTRIBUTIONS_HAS_AVX2)
    return binomial_bernoulli_sum_avx2(sm, trials, threshold);
#else
    return fast::binomial_bernoulli_sum(sm, trials, p);
#endif
}

/// Tier C: AVX2 Bernoulli sums + four parallel streams; beta draw stays scalar.
inline void beta_binomial_sample_batch(
    int* out, std::size_t n, int trials, double alpha, double beta, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        SplitMix64Stream lanes[4];
        for (std::uint32_t lane = 0; lane < 4; ++lane) {
            lanes[lane].seed(lane_seed(seed, lane));
        }
        std::size_t i = 0;
        while (i + 4 <= n) {
            out[i] = beta_binomial_sample_lane(lanes[0], trials, alpha, beta);
            out[i + 1] = beta_binomial_sample_lane(lanes[1], trials, alpha, beta);
            out[i + 2] = beta_binomial_sample_lane(lanes[2], trials, alpha, beta);
            out[i + 3] = beta_binomial_sample_lane(lanes[3], trials, alpha, beta);
            i += 4;
        }
        SplitMix64Stream tail;
        tail.seed(lane_seed(seed, 4));
        while (i < n) {
            out[i++] = beta_binomial_sample_lane(tail, trials, alpha, beta);
        }
        return;
    }
#endif
    fast::beta_binomial_sample_batch(out, n, trials, alpha, beta, seed);
}

}  // namespace distributions::detail::simd
