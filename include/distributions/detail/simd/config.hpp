#pragma once

#include <cstdint>

#if defined(DISTRIBUTIONS_HAS_AVX512) && defined(_MSC_VER)
#include <immintrin.h>
#endif

namespace distributions::detail::simd {

#if defined(DISTRIBUTIONS_HAS_AVX2)

#if defined(_MSC_VER)
#include <intrin.h>
inline void cpuid_ex(int info[4], int leaf, int subleaf) {
    __cpuidex(info, leaf, subleaf);
}
#elif defined(__GNUC__) || defined(__clang__)
#include <cpuid.h>
inline void cpuid_ex(int info[4], int leaf, int subleaf) {
    unsigned eax = 0;
    unsigned ebx = 0;
    unsigned ecx = 0;
    unsigned edx = 0;
    __cpuid_count(static_cast<unsigned>(leaf), static_cast<unsigned>(subleaf), eax, ebx, ecx, edx);
    info[0] = static_cast<int>(eax);
    info[1] = static_cast<int>(ebx);
    info[2] = static_cast<int>(ecx);
    info[3] = static_cast<int>(edx);
}
#else
inline void cpuid_ex(int info[4], int /*leaf*/, int /*subleaf*/) {
    info[0] = info[1] = info[2] = info[3] = 0;
}
#endif

/// Runtime check: CPU supports AVX2 (leaf 7, subleaf 0, EBX bit 5).
[[nodiscard]] inline bool cpu_has_avx2() noexcept {
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    int info[4]{};
    cpuid_ex(info, 0, 0);
    if (info[0] < 7) {
        return false;
    }
    cpuid_ex(info, 7, 0);
    return (static_cast<unsigned>(info[1]) & (1U << 5)) != 0U;
#else
    return false;
#endif
}

/// Tier C may run: compiled with AVX2 and CPU supports it.
[[nodiscard]] inline bool tier_c_enabled() noexcept {
    return cpu_has_avx2();
}

#if defined(DISTRIBUTIONS_HAS_AVX512)

[[nodiscard]] inline unsigned long long read_xcr0() noexcept {
#if defined(_MSC_VER)
    return _xgetbv(0);
#elif defined(__GNUC__) || defined(__clang__)
    unsigned int eax = 0;
    unsigned int edx = 0;
    __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
    return (static_cast<unsigned long long>(edx) << 32) | eax;
#else
    return 0ULL;
#endif
}

/// Runtime check: CPU and OS support AVX-512F (leaf 7 EBX bit 16 + XCR0 ZMM state).
[[nodiscard]] inline bool cpu_has_avx512() noexcept {
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    if (!cpu_has_avx2()) {
        return false;
    }
    int info[4]{};
    cpuid_ex(info, 1, 0);
    if ((static_cast<unsigned>(info[2]) & (1U << 27)) == 0U) {
        return false;
    }
    cpuid_ex(info, 7, 0);
    if ((static_cast<unsigned>(info[1]) & (1U << 16)) == 0U) {
        return false;
    }
    const unsigned long long xcr0 = read_xcr0();
    constexpr unsigned long long kZmmMask = 0xE6ULL;
    return (xcr0 & kZmmMask) == kZmmMask;
#else
    return false;
#endif
}

/// Tier C512 may run: compiled with AVX-512 and CPU supports it.
[[nodiscard]] inline bool tier_c512_enabled() noexcept {
    return cpu_has_avx512();
}

#else

[[nodiscard]] inline bool cpu_has_avx512() noexcept {
    return false;
}

[[nodiscard]] inline bool tier_c512_enabled() noexcept {
    return false;
}

#endif

#else

[[nodiscard]] inline bool cpu_has_avx2() noexcept {
    return false;
}

[[nodiscard]] inline bool tier_c_enabled() noexcept {
    return false;
}

[[nodiscard]] inline bool cpu_has_avx512() noexcept {
    return false;
}

[[nodiscard]] inline bool tier_c512_enabled() noexcept {
    return false;
}

#endif

}  // namespace distributions::detail::simd
