#pragma once

#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Kent {
    double kappa_;
    double beta_;
    Kent(double kappa, double beta) : kappa_(kappa), beta_(beta) {
        detail::assert_nonnegative(kappa_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_kent_x(rng, kappa_, beta_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
