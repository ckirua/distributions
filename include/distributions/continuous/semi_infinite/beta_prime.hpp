#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct BetaPrime {
    double alpha_;
    double beta_;
    BetaPrime(double alpha, double beta) : alpha_(alpha), beta_(beta) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double x = detail::sample_gamma(rng, alpha_, 1.0);
        const double y = detail::sample_gamma(rng, beta_, 1.0);
        return x / y;
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
