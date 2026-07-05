#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/uniform.hpp"
#include "distributions/detail/simd/config.hpp"
#include "distributions/detail/simd/uniform.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace {

bool moments_ok(const double* data, std::size_t n) {
    double sum = 0.0;
    double sum_sq = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        sum += data[i];
        sum_sq += data[i] * data[i];
    }
    const double mean = sum / static_cast<double>(n);
    const double var = sum_sq / static_cast<double>(n) - mean * mean;
    if (std::abs(mean - 0.5) > 0.005) {
        std::cerr << "mean out of range: " << mean << '\n';
        return false;
    }
    if (std::abs(var - 1.0 / 12.0) > 0.005) {
        std::cerr << "variance out of range: " << var << '\n';
        return false;
    }
    return true;
}

}  // namespace

int main() {
    constexpr std::size_t n = 1'000'000;
    std::vector<double> avx(n);
    std::vector<double> scalar(n);

    distributions::detail::simd::fill_uniform01_avx2(avx.data(), n, 12345, 0);
    if (!moments_ok(avx.data(), n)) {
        return EXIT_FAILURE;
    }

#if defined(DISTRIBUTIONS_HAS_AVX2)
    if (distributions::detail::simd::tier_c_enabled()) {
        distributions::detail::fast::fill_uniform01(scalar.data(), n, 12345, 0);
        for (std::size_t i = 0; i < n; ++i) {
            if (avx[i] != scalar[i]) {
                std::cerr << "mismatch at " << i << ": avx=" << avx[i] << " scalar=" << scalar[i]
                          << '\n';
                return EXIT_FAILURE;
            }
        }

        distributions::detail::CounterRng rng;
        rng.seed(777, 2);
        distributions::detail::simd::Philox4x32Avx2 avx_rng;
        avx_rng.seed(777, 2);
        for (std::size_t i = 0; i < 10'000; ++i) {
            if (rng.next_double() != avx_rng.next_double()) {
                std::cerr << "stream mismatch at draw " << i << '\n';
                return EXIT_FAILURE;
            }
        }
    } else {
        std::cout << "skip: CPU lacks AVX2 — fallback path only\n";
    }
#else
    distributions::detail::fast::fill_uniform01(scalar.data(), n, 12345, 0);
    for (std::size_t i = 0; i < n; ++i) {
        if (avx[i] != scalar[i]) {
            std::cerr << "fallback fill mismatch at " << i << '\n';
            return EXIT_FAILURE;
        }
    }
#endif

    return EXIT_SUCCESS;
}
