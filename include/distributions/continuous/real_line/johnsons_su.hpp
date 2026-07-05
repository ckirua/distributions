#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct JohnsonsSu {
    double a_;
    double b_;
    double loc_;
    double scale_;
    JohnsonsSu(double a, double b, double loc, double scale) : a_(a), b_(b), loc_(loc), scale_(scale) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(b_);
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_johnson_su(rng, a_, b_, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
