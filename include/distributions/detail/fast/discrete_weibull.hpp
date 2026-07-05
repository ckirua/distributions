#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace distributions::detail::fast {

inline void discrete_weibull_sample_batch(
    double* out, std::size_t n, double c, std::uint64_t seed) {
    static constexpr int kMin = -50;
    static constexpr int kMax = 50;
    static constexpr int kLen = kMax - kMin + 1;
    double pmf[kLen];
    double total = 0.0;
    for (int k = kMin; k <= kMax; ++k) {
        pmf[k - kMin] = std::pow(static_cast<double>(std::abs(k)), c - 1.0);
        total += pmf[k - kMin];
    }
    int alias[kLen];
    double prob[kLen];
    std::vector<int> small;
    std::vector<int> large;
    for (int i = 0; i < kLen; ++i) {
        prob[i] = pmf[i] / total * static_cast<double>(kLen);
        alias[i] = i;
        if (prob[i] < 1.0) {
            small.push_back(i);
        } else {
            large.push_back(i);
        }
    }
    while (!small.empty() && !large.empty()) {
        const int s = small.back();
        small.pop_back();
        const int l = large.back();
        large.pop_back();
        alias[s] = l;
        prob[l] = prob[l] + prob[s] - 1.0;
        if (prob[l] < 1.0) {
            small.push_back(l);
        } else {
            large.push_back(l);
        }
    }
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        const int col = static_cast<int>(rng.next_u32() % static_cast<std::uint32_t>(kLen));
        const double u = rng.next_double();
        const int j = u < prob[col] ? col : alias[col];
        out[i] = static_cast<double>(j + kMin);
    }
}

}  // namespace distributions::detail::fast
