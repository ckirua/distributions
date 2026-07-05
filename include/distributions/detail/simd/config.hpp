#pragma once

#include <cstdint>

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

#else

[[nodiscard]] inline bool cpu_has_avx2() noexcept {
    return false;
}

[[nodiscard]] inline bool tier_c_enabled() noexcept {
    return false;
}

#endif

}  // namespace distributions::detail::simd
