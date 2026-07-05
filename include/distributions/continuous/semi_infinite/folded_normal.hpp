#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct FoldedNormal {
    double c_;
    double loc_;
    double scale_;
    FoldedNormal(double c, double loc, double scale) : c_(c), loc_(loc), scale_(scale) {
        detail::assert_strictly_positive(c_);
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        for (;;) {
            const double x = loc_ + scale_ * detail::sample_standard_normal(rng);
            if (x >= c_) { return x; }
        }
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
