#pragma once

#include "distributions/detail/fast/normal.hpp"
#include "distributions/detail/simd/common.hpp"
#include "distributions/detail/simd/config.hpp"

#if defined(DISTRIBUTIONS_HAS_AVX2)
#include "distributions/detail/simd/libmvec.hpp"
#include <immintrin.h>
#endif

#if defined(DISTRIBUTIONS_HAS_AVX512)
#include "distributions/detail/simd/avx512/normal.hpp"
#endif

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::simd {

inline void normal_sample_batch(
    double* out, std::size_t n, double mu, double sigma, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX512)
    if (tier_c512_enabled()) {
        avx512::normal_sample_batch(out, n, mu, sigma, seed);
        return;
    }
#endif
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

/// Tier C for ``float`` output (8-wide AVX2; falls back to Tier B scalar).
inline void normal_sample_batch(
    float* out, std::size_t n, double mu, double sigma, std::uint64_t seed) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (tier_c_enabled()) {
        static constexpr float kTwoPi = 6.283185307179586f;
        static constexpr float kMinU1 = 1e-7f;
        const __m256 two_pi = _mm256_set1_ps(kTwoPi);
        const __m256 neg_two = _mm256_set1_ps(-2.0f);
        SplitMix64Stream rng;
        rng.seed(seed);
        std::size_t i = 0;
        while (i < n) {
            if (i + 8 <= n) {
                float u1a[4];
                float u2a[4];
                for (int k = 0; k < 4; ++k) {
                    u1a[k] = std::max(static_cast<float>(rng.next_double()), kMinU1);
                    u2a[k] = static_cast<float>(rng.next_double());
                }
                const __m256 u1 = _mm256_set_ps(
                    u1a[3], u1a[3], u1a[2], u1a[2], u1a[1], u1a[1], u1a[0], u1a[0]);
                const __m256 u2 = _mm256_set_ps(
                    u2a[3], u2a[3], u2a[2], u2a[2], u2a[1], u2a[1], u2a[0], u2a[0]);
                const __m256 r =
                    libmvec::sqrt_ps(_mm256_mul_ps(neg_two, libmvec::log_ps(u1)));
                const __m256 theta = _mm256_mul_ps(two_pi, u2);
                const __m256 c = libmvec::cos_ps(theta);
                const __m256 s = libmvec::sin_ps(theta);
                const __m256 mu_v = _mm256_set1_ps(static_cast<float>(mu));
                const __m256 sigma_v = _mm256_set1_ps(static_cast<float>(sigma));
                const __m256 sr = _mm256_mul_ps(sigma_v, r);
                const __m256 z_cos = _mm256_fmadd_ps(sr, c, mu_v);
                const __m256 z_sin = _mm256_fmadd_ps(sr, s, mu_v);
                alignas(32) float zc[8];
                alignas(32) float zs[8];
                _mm256_storeu_ps(zc, z_cos);
                _mm256_storeu_ps(zs, z_sin);
                for (int k = 0; k < 4; ++k) {
                    out[i + static_cast<std::size_t>(2 * k)] = zc[2 * k];
                    out[i + static_cast<std::size_t>(2 * k + 1)] = zs[2 * k];
                }
                i += 8;
            } else {
                static constexpr float kMinU1 = 1e-7f;
                const float u1 = std::max(static_cast<float>(rng.next_double()), kMinU1);
                const float u2 = static_cast<float>(rng.next_double());
                const float r = std::sqrt(-2.0f * std::log(u1));
                const float theta = kTwoPi * u2;
                out[i++] = static_cast<float>(mu) + static_cast<float>(sigma) * r * std::cos(theta);
                if (i < n) {
                    out[i++] =
                        static_cast<float>(mu) + static_cast<float>(sigma) * r * std::sin(theta);
                }
            }
        }
        return;
    }
#endif
    fast::normal_sample_batch(out, n, mu, sigma, seed);
}

}  // namespace distributions::detail::simd
