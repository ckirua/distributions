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

}  // namespace distributions::detail::simd::libmvec

#endif
