#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Beta {
    double alpha_;
    double beta_;
    Beta(double alpha, double beta) : alpha_(alpha), beta_(beta) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_beta(rng, alpha_, beta_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
