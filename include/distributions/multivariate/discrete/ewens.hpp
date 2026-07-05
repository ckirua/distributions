#pragma once

#include "distributions/detail/multivariate_discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Ewens {
    double theta_;
    int n_;
    Ewens(double theta, int n) : theta_(theta), n_(n) {
        detail::assert_finite(theta_);
        detail::assert_nonnegative_int(n_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_ewens_first(rng, theta_, n_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
