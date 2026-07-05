#pragma once

#include "distributions/detail/multivariate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MultivariateStable {
    double alpha_;
    double beta_;
    MultivariateStable(double alpha, double beta) : alpha_(alpha), beta_(beta) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_multivariate_stable_first(rng, alpha_, beta_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
