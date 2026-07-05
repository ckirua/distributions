#pragma once

#include "distributions/detail/fast/normal.hpp"
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

inline void normal_sample_batch(
    double* out, std::size_t n, double mu, double sigma, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        static constexpr double kTwoPi = 6.283185307179586;
        const __m256d two_pi = _mm256_set1_pd(kTwoPi);
        const __m256d neg_two = _mm256_set1_pd(-2.0);
        SplitMix64Stream rng;
        rng.seed(seed);
        std::size_t i = 0;
        while (i < n) {
            if (i + 4 <= n) {
                const double u1a = std::max(rng.next_double(), 1e-300);
                const double u2a = rng.next_double();
                const double u1b = std::max(rng.next_double(), 1e-300);
                const double u2b = rng.next_double();
                const __m256d u1 = _mm256_set_pd(u1b, u1b, u1a, u1a);
                const __m256d u2 = _mm256_set_pd(u2b, u2b, u2a, u2a);
                const __m256d r =
                    libmvec::sqrt_pd(_mm256_mul_pd(neg_two, libmvec::log_pd(u1)));
                const __m256d theta = _mm256_mul_pd(two_pi, u2);
                const __m256d c = libmvec::cos_pd(theta);
                const __m256d s = libmvec::sin_pd(theta);
                alignas(32) double rv[4];
                alignas(32) double cv[4];
                alignas(32) double sv[4];
                _mm256_store_pd(rv, r);
                _mm256_store_pd(cv, c);
                _mm256_store_pd(sv, s);
                out[i] = mu + sigma * rv[0] * cv[0];
                out[i + 1] = mu + sigma * rv[0] * sv[0];
                out[i + 2] = mu + sigma * rv[2] * cv[2];
                out[i + 3] = mu + sigma * rv[2] * sv[2];
                i += 4;
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
        return;
    }
#endif
    fast::normal_sample_batch(out, n, mu, sigma, seed);
}

}  // namespace distributions::detail::simd
