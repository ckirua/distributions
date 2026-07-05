#pragma once

#include "distributions/detail/fast/binomial.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/config.hpp"

#if defined(DISTRIBUTIONS_HAS_AVX2)
#include <immintrin.h>
#endif

#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

#if defined(DISTRIBUTIONS_HAS_AVX2)
inline int binomial_bernoulli_sum_avx2(
    SplitMix64Stream& rng, int trials, std::uint64_t threshold) {
    const __m256i thresh = _mm256_set1_epi64x(static_cast<std::int64_t>(threshold));
    int sum = 0;
    int t = 0;
    for (; t + 4 <= trials; t += 4) {
        const __m256i raw = _mm256_set_epi64x(
            static_cast<std::int64_t>(rng.next_u64()),
            static_cast<std::int64_t>(rng.next_u64()),
            static_cast<std::int64_t>(rng.next_u64()),
            static_cast<std::int64_t>(rng.next_u64()));
        const __m256i shifted = _mm256_srli_epi64(raw, 11);
        const __m256i cmp = _mm256_cmpgt_epi64(thresh, shifted);
        sum += _mm_popcnt_u32(
            static_cast<unsigned>(_mm256_movemask_pd(_mm256_castsi256_pd(cmp))));
    }
    for (; t < trials; ++t) {
        sum += (rng.next_u64() >> 11U) < threshold ? 1 : 0;
    }
    return sum;
}
#endif

/// Tier C: AVX2 Bernoulli trial sums + four parallel output streams.
inline void binomial_sample_batch(
    int* out, std::size_t n, int trials, double p, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        const std::uint64_t threshold =
            static_cast<std::uint64_t>(p * static_cast<double>(1ULL << 53));
        SplitMix64Stream lanes[4];
        for (std::uint32_t lane = 0; lane < 4; ++lane) {
            lanes[lane].seed(lane_seed(seed, lane));
        }
        std::size_t i = 0;
        while (i + 4 <= n) {
            out[i] = binomial_bernoulli_sum_avx2(lanes[0], trials, threshold);
            out[i + 1] = binomial_bernoulli_sum_avx2(lanes[1], trials, threshold);
            out[i + 2] = binomial_bernoulli_sum_avx2(lanes[2], trials, threshold);
            out[i + 3] = binomial_bernoulli_sum_avx2(lanes[3], trials, threshold);
            i += 4;
        }
        SplitMix64Stream tail;
        tail.seed(lane_seed(seed, 4));
        while (i < n) {
            out[i++] = binomial_bernoulli_sum_avx2(tail, trials, threshold);
        }
        return;
    }
#endif
    fast::binomial_sample_batch(out, n, trials, p, seed);
}

}  // namespace distributions::detail::simd
