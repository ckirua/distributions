#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct InverseGamma {
    double shape_;
    double scale_;
    InverseGamma(double shape, double scale) : shape_(shape), scale_(scale) {
        detail::assert_strictly_positive(shape_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return scale_ / detail::sample_gamma(rng, shape_, 1.0);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
