#include "distributions/detail/simd/config.hpp"

#include <cstdlib>
#include <iostream>

int main() {
#if defined(DISTRIBUTIONS_HAS_AVX512)
    if (!distributions::detail::simd::cpu_has_avx512()) {
        std::cout << "skip: CPU lacks AVX-512\n";
        return EXIT_SUCCESS;
    }
    if (!distributions::detail::simd::tier_c512_enabled()) {
        std::cerr << "tier_c512_enabled() false on AVX-512 CPU\n";
        return EXIT_FAILURE;
    }
    if (!distributions::detail::simd::tier_c_enabled()) {
        std::cerr << "tier_c_enabled() false on AVX-512 CPU\n";
        return EXIT_FAILURE;
    }
#else
    if (distributions::detail::simd::tier_c512_enabled()) {
        std::cerr << "tier_c512_enabled() true without DISTRIBUTIONS_HAS_AVX512\n";
        return EXIT_FAILURE;
    }
#endif
    return EXIT_SUCCESS;
}
