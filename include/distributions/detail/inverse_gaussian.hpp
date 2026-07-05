#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

// Michael-Schucany-Haas algorithm.
inline double sample_inverse_gaussian(Pcg32& rng, double mu, double lambda) {
    const double nu = sample_standard_normal(rng);
    const double y = nu * nu;
    const double x = mu + (mu * mu * y) / (2.0 * lambda) -
                     (mu / (2.0 * lambda)) * std::sqrt(4.0 * mu * lambda * y + mu * mu * y * y);
    const double u = rng.next_double();
    if (u <= mu / (mu + x)) {
        return x;
    }
    return mu * mu / x;
}

}  // namespace distributions::detail
