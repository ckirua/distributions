#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/rng.hpp"

namespace distributions::detail {

inline double sample_chi_squared(Pcg32& rng, double df) {
    return 2.0 * sample_gamma(rng, df * 0.5, 1.0);
}

inline double sample_chi(Pcg32& rng, double df) {
    return std::sqrt(sample_chi_squared(rng, df));
}

}  // namespace distributions::detail
