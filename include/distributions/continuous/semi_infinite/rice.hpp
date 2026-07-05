#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Rice {
    double b_;
    double scale_;
    Rice(double b, double scale) : b_(b), scale_(scale) {
        detail::assert_strictly_positive(b_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double z1 = detail::sample_standard_normal(rng);
        const double z2 = detail::sample_standard_normal(rng);
        return scale_ * std::sqrt(std::pow(b_ + z1, 2) + z2 * z2);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
