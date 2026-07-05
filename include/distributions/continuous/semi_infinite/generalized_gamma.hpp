#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct GeneralizedGamma {
    double a_;
    double c_;
    double scale_;
    GeneralizedGamma(double a, double c, double scale) : a_(a), c_(c), scale_(scale) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(c_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return scale_ * std::pow(detail::sample_gamma(rng, a_, 1.0), 1.0 / c_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
