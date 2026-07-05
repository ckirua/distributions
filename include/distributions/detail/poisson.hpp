#pragma once

#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

inline int sample_poisson(Pcg32& rng, double lambda) {
    if (lambda <= 0.0) {
        return 0;
    }
    if (lambda > 30.0) {
        // Normal approximation for large lambda.
        const double x = lambda + std::sqrt(lambda) * (rng.next_double() * 2.0 - 1.0) * 3.0;
        return x < 0.0 ? 0 : static_cast<int>(std::lround(x));
    }
    const double limit = std::exp(-lambda);
    double p = 1.0;
    int k = 0;
    do {
        ++k;
        p *= rng.next_double();
    } while (p > limit);
    return k - 1;
}

}  // namespace distributions::detail
