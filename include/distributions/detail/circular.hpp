#pragma once

#include "distributions/rng.hpp"

#include <cmath>
#include <numbers>

namespace distributions::detail {

inline double wrap_angle(double theta) {
    const double two_pi = 2.0 * std::numbers::pi;
    return std::fmod(theta + std::numbers::pi, two_pi) - std::numbers::pi;
}

inline double sample_wrapcauchy(Pcg32& rng, double c, double loc, double scale) {
    const double u = rng.next_double();
    const double val = (1.0 - c) / (1.0 + c);
    const double x = u < 0.5
        ? 2.0 * std::atan(val * std::tan(std::numbers::pi * u))
        : 2.0 * std::numbers::pi - 2.0 * std::atan(val * std::tan(std::numbers::pi * (1.0 - u)));
    return loc + scale * x;
}

}  // namespace distributions::detail
