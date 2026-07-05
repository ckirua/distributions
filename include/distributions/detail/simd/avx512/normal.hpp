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
inline void normal_sample_batch(
    double* out, std::size_t n, double mu, double sigma, std::uint64_t seed) {
    static constexpr double kTwoPi = 6.283185307179586;
    const __m512d two_pi = _mm512_set1_pd(kTwoPi);
    const __m512d neg_two = _mm512_set1_pd(-2.0);
    SplitMix64Stream rng;
    rng.seed(seed);
    std::size_t i = 0;
    while (i < n) {
        if (i + 8 <= n) {
            const double u1a = std::max(rng.next_double(), 1e-300);
            const double u2a = rng.next_double();
            const double u1b = std::max(rng.next_double(), 1e-300);
            const double u2b = rng.next_double();
            const double u1c = std::max(rng.next_double(), 1e-300);
            const double u2c = rng.next_double();
            const double u1d = std::max(rng.next_double(), 1e-300);
            const double u2d = rng.next_double();
            const __m512d u1 =
                _mm512_set_pd(u1d, u1d, u1c, u1c, u1b, u1b, u1a, u1a);
            const __m512d u2 =
                _mm512_set_pd(u2d, u2d, u2c, u2c, u2b, u2b, u2a, u2a);
            const __m512d r =
                libmvec::sqrt_pd512(_mm512_mul_pd(neg_two, libmvec::log_pd512(u1)));
            const __m512d theta = _mm512_mul_pd(two_pi, u2);
            const __m512d c = libmvec::cos_pd512(theta);
            const __m512d s = libmvec::sin_pd512(theta);
            alignas(64) double rv[8];
            alignas(64) double cv[8];
            alignas(64) double sv[8];
            _mm512_store_pd(rv, r);
            _mm512_store_pd(cv, c);
            _mm512_store_pd(sv, s);
            for (int k = 0; k < 4; ++k) {
                const std::size_t base = i + static_cast<std::size_t>(2 * k);
                out[base] = mu + sigma * rv[static_cast<std::size_t>(2 * k)] *
                                    cv[static_cast<std::size_t>(2 * k)];
                out[base + 1] = mu + sigma * rv[static_cast<std::size_t>(2 * k)] *
                                         sv[static_cast<std::size_t>(2 * k)];
            }
            i += 8;
        } else {
            const double u1 = std::max(rng.next_double(), 1e-300);
            const double u2 = rng.next_double();
            const double r = std::sqrt(-2.0 * std::log(u1));
            const double theta = kTwoPi * u2;
            out[i++] = mu + sigma * r * std::cos(theta);
            if (i < n) {
                out[i++] = mu + sigma * r * std::sin(theta);
            }
        }
    }
}

}  // namespace distributions::detail::simd::avx512
