#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void continuous_bernoulli_sample_batch(
    double* out, std::size_t n, double lambda, std::uint64_t seed) {
    const double lam = std::min(std::max(lambda, 1e-6), 1.0 - 1e-6);
    static constexpr int kGrid = 512;
    double weights[kGrid];
    double total = 0.0;
    for (int i = 0; i < kGrid; ++i) {
        const double x = (static_cast<double>(i) + 0.5) / static_cast<double>(kGrid);
        weights[i] = std::exp(x * std::log(lam) + (1.0 - x) * std::log1p(-lam));
        total += weights[i];
    }
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        const double u = rng.next_double() * total;
        double cumulative = 0.0;
        for (int j = 0; j < kGrid; ++j) {
            cumulative += weights[j];
            if (u <= cumulative) {
                out[i] = (static_cast<double>(j) + 0.5) / static_cast<double>(kGrid);
                break;
            }
        }
    }
}

}  // namespace distributions::detail::fast
