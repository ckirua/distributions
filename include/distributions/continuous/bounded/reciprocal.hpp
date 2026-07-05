#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Reciprocal {
    double a_;
    double b_;
    Reciprocal(double a, double b) : a_(a), b_(b) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(b_);
        detail::assert_double_interval(a_, b_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return std::exp(detail::sample_uniform(rng, std::log(a_), std::log(b_)));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
