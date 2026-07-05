#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Kumaraswamy {
    double a_;
    double b_;
    Kumaraswamy(double a, double b) : a_(a), b_(b) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
        const double v = detail::sample_uniform(rng, 0.0, 1.0);
        return std::pow(1.0 - std::pow(1.0 - u, 1.0 / a_), 1.0 / b_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
