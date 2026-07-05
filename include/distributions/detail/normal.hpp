#pragma once

#include "distributions/rng.hpp"

#include <cmath>
#include <numbers>

namespace distributions::detail {

inline double sample_standard_normal(Pcg32& rng) {
    const double u1 = std::max(rng.next_double(), 1e-300);
    const double u2 = rng.next_double();
    return std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * std::numbers::pi * u2);
}

inline double sample_normal(Pcg32& rng, double mu, double sigma) {
    return mu + sigma * sample_standard_normal(rng);
}

}  // namespace distributions::detail
