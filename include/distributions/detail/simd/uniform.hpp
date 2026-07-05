#pragma once

#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/uniform.hpp"
#include "distributions/detail/simd/config.hpp"

#if defined(DISTRIBUTIONS_HAS_AVX2)
#include "distributions/detail/simd/philox4x32_avx2.hpp"
#include <immintrin.h>
#endif

#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

/// Tier-C uniform fill on [0, 1). Falls back to scalar Philox when SIMD is unavailable.
inline void fill_uniform01_avx2(
    double* out, std::size_t n, std::uint64_t seed, std::uint32_t stream = 0) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        Philox4x32Avx2 rng;
        rng.seed(seed, stream);
        std::size_t i = 0;
        while (i + 4 <= n) {
            const __m256d u = rng.generate_u01_block();
            _mm256_storeu_pd(out + i, u);
            i += 4;
        }
        while (i < n) {
            out[i++] = rng.next_double();
        }
        return;
    }
#endif
    fast::fill_uniform01(out, n, seed, stream);
}

}  // namespace distributions::detail::simd
