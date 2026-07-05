#pragma once

#if defined(DISTRIBUTIONS_HAS_AVX512)

#if defined(__GNUC__) || defined(__clang__)
#define DISTRIBUTIONS_AVX512_TARGET __attribute__((target("avx512f")))
#elif defined(_MSC_VER)
#define DISTRIBUTIONS_AVX512_TARGET __declspec(target("avx512f"))
#else
#define DISTRIBUTIONS_AVX512_TARGET
#endif

#else

#define DISTRIBUTIONS_AVX512_TARGET

#endif
