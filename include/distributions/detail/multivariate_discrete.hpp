#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <vector>

namespace distributions::detail {

inline int sample_binomial(Pcg32& rng, int n, double p) {
    int x = 0;
    for (int i = 0; i < n; ++i) {
        if (rng.next_double() < p) {
            ++x;
        }
    }
    return x;
}

inline double sample_multinomial_first(Pcg32& rng, int n, double p0) {
    return static_cast<double>(sample_binomial(rng, n, p0));
}

inline double sample_dirichlet_multinomial_first(Pcg32& rng, int n, double a0, double a1, double a2) {
    const double p = sample_beta(rng, a0, a1 + a2);
    return static_cast<double>(sample_binomial(rng, n, p));
}

inline double sample_negative_multinomial_first(Pcg32& rng, double r, double p0, double p1, double p2) {
    (void)p1;
    const double g = sample_gamma(rng, r, 1.0);
    return static_cast<double>(sample_poisson(rng, g * p0 / std::max(p2, 1e-12)));
}

inline double sample_ewens_first(Pcg32& rng, double theta, int n) {
    std::vector<int> counts(1, 1);
    for (int i = 2; i <= n; ++i) {
        const double u = rng.next_double() * (theta + static_cast<double>(i - 1));
        double cumulative = theta;
        std::size_t chosen = counts.size();
        for (std::size_t j = 0; j < counts.size(); ++j) {
            cumulative += static_cast<double>(counts[j]);
            if (u <= cumulative) {
                chosen = j;
                break;
            }
        }
        if (chosen == counts.size()) {
            counts.push_back(1);
        } else {
            counts[chosen]++;
        }
    }
    return static_cast<double>(counts[0]);
}

}  // namespace distributions::detail
