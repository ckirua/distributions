#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct DiscreteWeibull {
    double c_;
    DiscreteWeibull(double c) : c_(c) {
        detail::assert_strictly_positive(c_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_discrete_weibull(rng, c_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
