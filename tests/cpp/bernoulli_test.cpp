#include "distributions/discrete/finite/bernoulli.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

int main() {
    constexpr double p = 0.35;
    distributions::Bernoulli dist(p);
    distributions::Pcg32 rng(12345);

    double sum = 0.0;
    double sum_sq = 0.0;
    constexpr int n = 2'000;  // below kFastThreshold — Tier A bit-identical check
    std::vector<int> samples;
    samples.reserve(static_cast<std::size_t>(n));

    for (int i = 0; i < n; ++i) {
        const int x = dist.sample(rng);
        samples.push_back(x);
        sum += x;
        sum_sq += static_cast<double>(x * x);
    }

    const double sample_mean = sum / static_cast<double>(n);
    const double sample_var =
        (sum_sq / static_cast<double>(n)) - sample_mean * sample_mean;

    if (std::abs(sample_mean - dist.mean()) > 0.02) {
        std::cerr << "mean mismatch: sample=" << sample_mean << " expected=" << dist.mean()
                  << '\n';
        return EXIT_FAILURE;
    }
    if (std::abs(sample_var - dist.variance()) > 0.02) {
        std::cerr << "variance mismatch: sample=" << sample_var
                  << " expected=" << dist.variance() << '\n';
        return EXIT_FAILURE;
    }

    std::vector<int> batch(static_cast<std::size_t>(n));
    distributions::Pcg32 batch_rng(12345);
    dist.sample_batch(batch.data(), batch.size(), batch_rng);
    for (std::size_t i = 0; i < batch.size(); ++i) {
        if (batch[i] != samples[i]) {
            std::cerr << "sample_batch mismatch at index " << i << '\n';
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
