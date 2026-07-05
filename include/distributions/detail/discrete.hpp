#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <vector>

namespace distributions::detail {

inline int sample_geometric(Pcg32& rng, double p) {
    return static_cast<int>(std::floor(std::log1p(-rng.next_double()) / std::log1p(-p))) + 1;
}

inline int sample_negative_binomial(Pcg32& rng, double r, double p) {
    int k = 0;
    double s = 0.0;
    while (s < r) {
        s += sample_gamma(rng, 1.0, 1.0);
        ++k;
    }
    int successes = 0;
    for (int i = 0; i < k - 1; ++i) {
        if (rng.next_double() < p) {
            ++successes;
        }
    }
    return successes;
}

inline int sample_logarithmic(Pcg32& rng, double p) {
    const double log1mp = std::log1p(-p);
    const double u = rng.next_double();
    double cumulative = 0.0;
    for (int k = 1; k < 10'000; ++k) {
        cumulative -= std::pow(p, static_cast<double>(k)) /
                      (static_cast<double>(k) * log1mp);
        if (u <= cumulative) {
            return k;
        }
    }
    return 1;
}

inline int sample_benford(Pcg32& rng) {
    const double u = rng.next_double();
    static constexpr double cdf[10] = {
        0.0,
        0.3010299956639812,
        0.47712125471966244,
        0.6020599913279624,
        0.6989700043360189,
        0.7781512503836436,
        0.8450980400142568,
        0.9030899869919436,
        0.9542425094393249,
        1.0,
    };
    for (int d = 1; d <= 9; ++d) {
        if (u <= cdf[d]) {
            return d;
        }
    }
    return 9;
}

inline int sample_rademacher(Pcg32& rng) {
    return rng.next_double() < 0.5 ? -1 : 1;
}

inline int sample_yule_simon(Pcg32& rng, double rho) {
    const double u = rng.next_double();
    const double v = rng.next_double();
    const double x = std::exp(-std::log(u) / rho);
    return static_cast<int>(std::floor(1.0 + std::log1p(-v) / std::log1p(-std::exp(-rho / x)))) + 1;
}

}  // namespace distributions::detail
