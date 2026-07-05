#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct ParabolicFractal {
    double b_;
    double c_;
    ParabolicFractal(double b, double c) : b_(b), c_(c) {
        detail::assert_strictly_positive(b_);
        detail::assert_strictly_positive(c_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_parabolic_fractal(rng, b_, c_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
