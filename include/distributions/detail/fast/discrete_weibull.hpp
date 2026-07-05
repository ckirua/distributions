#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void discrete_weibull_sample_batch(
    double* out, std::size_t n, double c, std::uint64_t seed) {
    static constexpr int kMin = -50;
    static constexpr int kMax = 50;
    static constexpr int kLen = kMax - kMin + 1;
    double pmf[kLen];
    double cumulative[kLen];
    double total = 0.0;
    for (int k = kMin; k <= kMax; ++k) {
        pmf[k - kMin] = std::pow(static_cast<double>(std::abs(k)), c - 1.0);
        total += pmf[k - kMin];
    }
    double run = 0.0;
    for (int i = 0; i < kLen; ++i) {
        run += pmf[i] / total;
        cumulative[i] = run;
    }
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        const double u = rng.next_double();
        for (int j = 0; j < kLen; ++j) {
            if (u <= cumulative[j]) {
                out[i] = static_cast<double>(j + kMin);
                break;
            }
        }
    }
}

}  // namespace distributions::detail::fast
