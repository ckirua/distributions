#include "distributions/detail/simd/config.hpp"

#include <cstdlib>
#include <iostream>

int main() {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (!distributions::detail::simd::cpu_has_avx2()) {
        std::cout << "skip: CPU lacks AVX2\n";
        return EXIT_SUCCESS;
    }
    if (!distributions::detail::simd::tier_c_enabled()) {
        std::cerr << "tier_c_enabled() false on AVX2 CPU\n";
        return EXIT_FAILURE;
    }
#else
    if (distributions::detail::simd::tier_c_enabled()) {
        std::cerr << "tier_c_enabled() true without DISTRIBUTIONS_HAS_AVX2\n";
        return EXIT_FAILURE;
    }
#endif
    return EXIT_SUCCESS;
}
