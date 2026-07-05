#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Nakagami {
    double nu_;
    double scale_;
    Nakagami(double nu, double scale) : nu_(nu), scale_(scale) {
        detail::assert_strictly_positive(nu_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return scale_ * std::sqrt(detail::sample_gamma(rng, nu_, 1.0));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
