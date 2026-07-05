#pragma once

#include "distributions/detail/fast/bernoulli.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/config.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

/// Tier C: four parallel SplitMix64 streams (same u64 compare as Tier B).
inline void bernoulli_sample_batch(int* out, std::size_t n, double p, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        const std::uint64_t threshold =
            static_cast<std::uint64_t>(p * static_cast<double>(1ULL << 53));
        SplitMix64Stream lanes[4];
        for (std::uint32_t k = 0; k < 4; ++k) {
            lanes[k].seed(lane_seed(seed, k));
        }
        std::size_t i = 0;
        while (i + 4 <= n) {
            out[i] = (lanes[0].next_u64() >> 11U) < threshold ? 1 : 0;
            out[i + 1] = (lanes[1].next_u64() >> 11U) < threshold ? 1 : 0;
            out[i + 2] = (lanes[2].next_u64() >> 11U) < threshold ? 1 : 0;
            out[i + 3] = (lanes[3].next_u64() >> 11U) < threshold ? 1 : 0;
            i += 4;
        }
        SplitMix64Stream tail;
        tail.seed(lane_seed(seed, 4));
        while (i < n) {
            out[i++] = (tail.next_u64() >> 11U) < threshold ? 1 : 0;
        }
        return;
    }
#endif
    fast::bernoulli_sample_batch(out, n, p, seed);
}

}  // namespace distributions::detail::simd
