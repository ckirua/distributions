#pragma once

#include "distributions/rng.hpp"

#include <cmath>
#include <numbers>

namespace distributions::detail {

// Marsaglia-Tsang for shape >= 1; shape < 1 uses boost with shape+1.
inline double sample_gamma(Pcg32& rng, double shape, double scale) {
    if (shape <= 0.0 || scale <= 0.0) {
        return 0.0;
    }
    if (shape < 1.0) {
        const double u = rng.next_double();
        return sample_gamma(rng, shape + 1.0, scale) * std::pow(u, 1.0 / shape);
    }
    const double d = shape - 1.0 / 3.0;
    const double c = 1.0 / std::sqrt(9.0 * d);
    for (;;) {
        double x = 0.0;
        double v = 0.0;
        do {
            const double u1 = rng.next_double();
            const double u2 = rng.next_double();
            x = std::sqrt(-2.0 * std::log(u1 + 1e-300)) *
                std::cos(2.0 * std::numbers::pi * u2);
            v = 1.0 + c * x;
        } while (v <= 0.0);
        v = v * v * v;
        const double u = rng.next_double();
        if (u < 1.0 - 0.0331 * (x * x) * (x * x)) {
            return d * v * scale;
        }
        if (std::log(u) < 0.5 * x * x + d * (1.0 - v + std::log(v))) {
            return d * v * scale;
        }
    }
}

inline double sample_beta(Pcg32& rng, double alpha, double beta) {
    const double x = sample_gamma(rng, alpha, 1.0);
    const double y = sample_gamma(rng, beta, 1.0);
    return x / (x + y);
}

}  // namespace distributions::detail
