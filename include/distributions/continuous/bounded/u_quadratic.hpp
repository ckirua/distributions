#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct UQuadratic {
    double a_;
    double b_;
    UQuadratic(double a, double b) : a_(a), b_(b) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
        const double t = a_ + (b_ - a_) * u;
        return 0.5 * (a_ + b_) + 0.5 * (b_ - a_) * (2.0 * t / (a_ + b_) - 1.0);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
