#pragma once

#include "distributions/detail/simd/avx512/target.hpp"

#include <immintrin.h>

namespace distributions::detail::simd::libmvec {

extern "C" __m512d _ZGVeN8v_log1p(__m512d);
extern "C" __m512d _ZGVeN8v_log(__m512d);
extern "C" __m512d _ZGVeN8v_cos(__m512d);
extern "C" __m512d _ZGVeN8v_sin(__m512d);

DISTRIBUTIONS_AVX512_TARGET
inline __m512d log1p_pd512(__m512d x) {
    return _ZGVeN8v_log1p(x);
}

DISTRIBUTIONS_AVX512_TARGET
inline __m512d log_pd512(__m512d x) {
    return _ZGVeN8v_log(x);
}

DISTRIBUTIONS_AVX512_TARGET
inline __m512d cos_pd512(__m512d x) {
    return _ZGVeN8v_cos(x);
}

DISTRIBUTIONS_AVX512_TARGET
inline __m512d sin_pd512(__m512d x) {
    return _ZGVeN8v_sin(x);
}

DISTRIBUTIONS_AVX512_TARGET
inline __m512d sqrt_pd512(__m512d x) {
    return _mm512_sqrt_pd(x);
}

}  // namespace distributions::detail::simd::libmvec
