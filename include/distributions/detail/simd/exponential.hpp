#pragma once

#include "distributions/detail/fast/exponential.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/config.hpp"

#if defined(DISTRIBUTIONS_HAS_AVX2)
#include "distributions/detail/simd/libmvec.hpp"
#include <immintrin.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

inline void exponential_sample_batch(
    double* out, std::size_t n, double rate, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        const double inv_rate = -1.0 / rate;
        const __m256d invv = _mm256_set1_pd(inv_rate);
        const __m256d zero = _mm256_setzero_pd();
        SplitMix64Stream rng;
        rng.seed(seed);
        constexpr std::size_t kBlock = 256;
        double uniforms[kBlock];
        std::size_t i = 0;
        while (i < n) {
            const std::size_t chunk = std::min(kBlock, n - i);
            rng.fill_uniform01(uniforms, chunk);
            std::size_t j = 0;
            for (; j + 4 <= chunk; j += 4) {
                const __m256d u = _mm256_loadu_pd(uniforms + j);
                const __m256d samples =
                    _mm256_mul_pd(libmvec::log1p_pd(_mm256_sub_pd(zero, u)), invv);
                _mm256_storeu_pd(out + i + j, samples);
            }
            for (; j < chunk; ++j) {
                out[i + j] = std::log1p(-uniforms[j]) * inv_rate;
            }
            i += chunk;
        }
        return;
    }
#endif
    fast::exponential_sample_batch(out, n, rate, seed);
}

}  // namespace distributions::detail::simd
