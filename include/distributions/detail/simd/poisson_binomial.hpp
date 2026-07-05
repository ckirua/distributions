#pragma once

#include "distributions/detail/fast/poisson_binomial.hpp"
#include "distributions/detail/simd/binomial.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/config.hpp"

#if defined(DISTRIBUTIONS_HAS_AVX2)
#include <immintrin.h>
#endif

#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

#if defined(DISTRIBUTIONS_HAS_AVX2)
inline int poisson_binomial_sum_avx2(
    SplitMix64Stream& rng, const double* probs, std::size_t k) {
    if (k == 4) {
        alignas(32) std::int64_t thresh_arr[4];
        for (std::size_t j = 0; j < 4; ++j) {
            thresh_arr[j] = static_cast<std::int64_t>(static_cast<std::uint64_t>(
                probs[j] * static_cast<double>(1ULL << 53)));
        }
        const __m256i thresh = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(thresh_arr));
        const __m256i raw = _mm256_set_epi64x(
            static_cast<std::int64_t>(rng.next_u64()),
            static_cast<std::int64_t>(rng.next_u64()),
            static_cast<std::int64_t>(rng.next_u64()),
            static_cast<std::int64_t>(rng.next_u64()));
        const __m256i shifted = _mm256_srli_epi64(raw, 11);
        const __m256i cmp = _mm256_cmpgt_epi64(thresh, shifted);
        return static_cast<int>(_mm_popcnt_u32(
            static_cast<unsigned>(_mm256_movemask_pd(_mm256_castsi256_pd(cmp)))));
    }
    int sum = 0;
    for (std::size_t j = 0; j < k; ++j) {
        const std::uint64_t threshold =
            static_cast<std::uint64_t>(probs[j] * static_cast<double>(1ULL << 53));
        sum += (rng.next_u64() >> 11U) < threshold ? 1 : 0;
    }
    return sum;
}
#endif

/// Tier C: vector trial compares when k==4; four parallel output streams.
inline void poisson_binomial_sample_batch(
    int* out,
    std::size_t n,
    const double* probs,
    std::size_t k,
    std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        SplitMix64Stream lanes[4];
        for (std::uint32_t lane = 0; lane < 4; ++lane) {
            lanes[lane].seed(lane_seed(seed, lane));
        }
        std::size_t i = 0;
        while (i + 4 <= n) {
            out[i] = poisson_binomial_sum_avx2(lanes[0], probs, k);
            out[i + 1] = poisson_binomial_sum_avx2(lanes[1], probs, k);
            out[i + 2] = poisson_binomial_sum_avx2(lanes[2], probs, k);
            out[i + 3] = poisson_binomial_sum_avx2(lanes[3], probs, k);
            i += 4;
        }
        SplitMix64Stream tail;
        tail.seed(lane_seed(seed, 4));
        while (i < n) {
            out[i++] = poisson_binomial_sum_avx2(tail, probs, k);
        }
        return;
    }
#endif
    fast::poisson_binomial_sample_batch(out, n, probs, k, seed);
}

}  // namespace distributions::detail::simd
