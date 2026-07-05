#pragma once

#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

// Sequential inverse-CDF (Fisher's non-central hypergeometric); fast for small N.
inline int sample_hypergeometric(Pcg32& rng, int M, int n, int N) {
    const int lo = std::max(0, N - (M - n));
    const int hi = std::min(n, N);
    const double u = rng.next_double();
    double cumulative = 0.0;
    double pmf = 1.0;
    for (int k = lo; k <= hi; ++k) {
        if (k > lo) {
            pmf *= static_cast<double>(n - k + 1) * static_cast<double>(N - k + 1) /
                   (static_cast<double>(k) * static_cast<double>(M - n - N + k));
        }
        cumulative += pmf;
        if (u <= cumulative) {
            return k;
        }
    }
    return hi;
}

}  // namespace distributions::detail
