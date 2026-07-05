#pragma once

#include "distributions/detail/circular.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct WrappedAsymmetricLaplace {
    double loc_;
    double scale_;
    WrappedAsymmetricLaplace(double loc, double scale) : loc_(loc), scale_(scale) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = rng.next_double();
        const double x = u < 0.5
            ? loc_ + scale_ * std::log(2.0 * u)
            : loc_ - scale_ * std::log(2.0 * (1.0 - u));
        return detail::wrap_angle(x);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
