#pragma once

#include "distributions/detail/multivariate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MultivariateNormal {
    double rho_;
    MultivariateNormal(double rho) : rho_(rho) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_multivariate_normal_first(rng, rho_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
