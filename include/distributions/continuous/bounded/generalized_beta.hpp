#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct GeneralizedBeta {
    double alpha_;
    double beta_;
    double lambda_;
    GeneralizedBeta(double alpha, double beta, double lambda) : alpha_(alpha), beta_(beta), lambda_(lambda) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
        detail::assert_nonnegative(lambda_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_generalized_beta(rng, alpha_, beta_, lambda_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
