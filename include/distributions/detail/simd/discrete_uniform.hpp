#pragma once

#include "distributions/detail/fast/discrete_uniform.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/config.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

/// Tier C: four parallel SplitMix64 streams (same transform as Tier B).
inline void discrete_uniform_sample_batch(
    int* out, std::size_t n, int low, int high, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        const double scale = static_cast<double>(high - low + 1);
        SplitMix64Stream lanes[4];
        for (std::uint32_t k = 0; k < 4; ++k) {
            lanes[k].seed(lane_seed(seed, k));
        }
        std::size_t i = 0;
        while (i + 4 <= n) {
            out[i] = low + static_cast<int>(lanes[0].next_double() * scale);
            out[i + 1] = low + static_cast<int>(lanes[1].next_double() * scale);
            out[i + 2] = low + static_cast<int>(lanes[2].next_double() * scale);
            out[i + 3] = low + static_cast<int>(lanes[3].next_double() * scale);
            i += 4;
        }
        SplitMix64Stream tail;
        tail.seed(lane_seed(seed, 4));
        while (i < n) {
            out[i++] = low + static_cast<int>(tail.next_double() * scale);
        }
        return;
    }
#endif
    fast::discrete_uniform_sample_batch(out, n, low, high, seed);
}

}  // namespace distributions::detail::simd
