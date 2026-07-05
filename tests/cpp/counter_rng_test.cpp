#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/uniform.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

int main() {
    using distributions::detail::CounterRng;
    using distributions::detail::fast::fill_uniform01;

    CounterRng a;
    a.seed(42, 0);
    CounterRng b;
    b.seed(42, 1);
    if (a.next_double() == b.next_double()) {
        std::cerr << "different streams produced identical first draw\n";
        return EXIT_FAILURE;
    }

    CounterRng c;
    c.seed(99, 0);
    const double r0 = c.next_double();
    const double r1 = c.next_double();
    if (r0 == r1) {
        std::cerr << "consecutive draws should not be identical\n";
        return EXIT_FAILURE;
    }

    constexpr std::size_t n = 1'000'000;
    double sum = 0.0;
    double sum_sq = 0.0;
    CounterRng stat;
    stat.seed(12345, 0);
    for (std::size_t i = 0; i < n; ++i) {
        const double u = stat.next_double();
        sum += u;
        sum_sq += u * u;
    }
    const double mean = sum / static_cast<double>(n);
    const double var = sum_sq / static_cast<double>(n) - mean * mean;
    if (std::abs(mean - 0.5) > 0.005) {
        std::cerr << "mean out of range: " << mean << '\n';
        return EXIT_FAILURE;
    }
    if (std::abs(var - 1.0 / 12.0) > 0.005) {
        std::cerr << "variance out of range: " << var << '\n';
        return EXIT_FAILURE;
    }

    std::vector<double> batch(n);
    fill_uniform01(batch.data(), n, 12345, 0);
    double batch_sum = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        batch_sum += batch[i];
    }
    const double batch_mean = batch_sum / static_cast<double>(n);
    if (std::abs(batch_mean - 0.5) > 0.005) {
        std::cerr << "batch mean out of range: " << batch_mean << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
