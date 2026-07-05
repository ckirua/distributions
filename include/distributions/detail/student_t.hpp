#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

inline double sample_student_t(Pcg32& rng, double df) {
    const double z = sample_standard_normal(rng);
    const double v = sample_gamma(rng, df * 0.5, 2.0);
    return z * std::sqrt(df / v);
}

}  // namespace distributions::detail
