#pragma once

#if defined(DISTRIBUTIONS_HAS_AVX2)

#include <immintrin.h>

namespace distributions::detail::simd::libmvec {

extern "C" __m256d _ZGVdN4v_log1p(__m256d);
extern "C" __m256d _ZGVdN4v_log(__m256d);
extern "C" __m256d _ZGVdN4v_cos(__m256d);
extern "C" __m256d _ZGVdN4v_sin(__m256d);

inline __m256d log1p_pd(__m256d x) {
    return _ZGVdN4v_log1p(x);
}

inline __m256d log_pd(__m256d x) {
    return _ZGVdN4v_log(x);
}

inline __m256d cos_pd(__m256d x) {
    return _ZGVdN4v_cos(x);
}

inline __m256d sin_pd(__m256d x) {
    return _ZGVdN4v_sin(x);
}

inline __m256d sqrt_pd(__m256d x) {
    return _mm256_sqrt_pd(x);
}

inline __m256d max_pd(__m256d a, __m256d b) {
    return _mm256_max_pd(a, b);
}

extern "C" __m256 _ZGVdN8v_log1pf(__m256);
extern "C" __m256 _ZGVdN8v_logf(__m256);
extern "C" __m256 _ZGVdN8v_cosf(__m256);
extern "C" __m256 _ZGVdN8v_sinf(__m256);

inline __m256 log1p_ps(__m256 x) {
    return _ZGVdN8v_log1pf(x);
}

inline __m256 log_ps(__m256 x) {
    return _ZGVdN8v_logf(x);
}

inline __m256 cos_ps(__m256 x) {
    return _ZGVdN8v_cosf(x);
}

inline __m256 sin_ps(__m256 x) {
    return _ZGVdN8v_sinf(x);
}

inline __m256 sqrt_ps(__m256 x) {
    return _mm256_sqrt_ps(x);
}

inline __m256 max_ps(__m256 a, __m256 b) {
    return _mm256_max_ps(a, b);
}

}  // namespace distributions::detail::simd::libmvec

#endif
