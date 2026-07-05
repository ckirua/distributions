// Example: C++ headers — single draw and batch sampling.
// Build: make build  →  ./build/example_cpp_basic

#include "distributions/continuous/real_line/normal.hpp"
#include "distributions/discrete/infinite/poisson.hpp"
#include "distributions/rng.hpp"

#include <cstdio>
#include <vector>

int main() {
    distributions::Pcg32 rng(42);

    // --- single samples ---
    distributions::Poisson poisson(4.0);
    const int k = poisson.sample(rng);
    std::printf("poisson sample: %d (E[X]=4)\n", k);

    distributions::Normal normal(0.0, 1.0);
    const double x = normal.sample(rng);
    std::printf("normal sample: %.4f\n", x);

    // --- batch into a stack buffer ---
    constexpr std::size_t n = 8;
    int discrete[n];
    poisson.sample_batch(discrete, n, rng);
    std::printf("poisson batch: ");
    for (std::size_t i = 0; i < n; ++i) {
        std::printf("%d ", discrete[i]);
    }
    std::printf("\n");

    // --- batch into std::vector (continuous) ---
    std::vector<double> continuous(n);
    normal.sample_batch(continuous.data(), n, rng);
    std::printf("normal batch: ");
    for (double v : continuous) {
        std::printf("%.3f ", v);
    }
    std::printf("\n");

    return 0;
}
