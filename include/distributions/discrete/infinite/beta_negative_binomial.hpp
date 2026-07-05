#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct BetaNegativeBinomial {
    double alpha_;
    double beta_;
    double r_;
    BetaNegativeBinomial(double alpha, double beta, double r) : alpha_(alpha), beta_(beta), r_(r) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
        detail::assert_strictly_positive(r_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_beta_negative_binomial(rng, alpha_, beta_, r_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
