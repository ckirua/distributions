#pragma once

#include "distributions/detail/simd/avx512/target.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/avx512/libmvec.hpp"

#include <immintrin.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd::avx512 {

DISTRIBUTIONS_AVX512_TARGET
inline void exponential_sample_batch(
    double* out, std::size_t n, double rate, std::uint64_t seed) {
    const __m512d invv = _mm512_set1_pd(-1.0 / rate);
    const __m512d zero = _mm512_setzero_pd();
    SplitMix64Stream rng;
    rng.seed(seed);
    constexpr std::size_t kBlock = 256;
    double uniforms[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(uniforms, chunk);
        std::size_t j = 0;
        for (; j + 8 <= chunk; j += 8) {
            const __m512d u = _mm512_loadu_pd(uniforms + j);
            const __m512d samples =
                _mm512_mul_pd(libmvec::log1p_pd512(_mm512_sub_pd(zero, u)), invv);
            _mm512_storeu_pd(out + i + j, samples);
        }
        for (; j < chunk; ++j) {
            out[i + j] = std::log1p(-uniforms[j]) * (-1.0 / rate);
        }
        i += chunk;
    }
}

}  // namespace distributions::detail::simd::avx512
