#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <numbers>
#include <cmath>
#include <cstddef>

namespace distributions {

struct RaisedCosine {
    double lo_;
    double hi_;
    RaisedCosine(double lo, double hi) : lo_(lo), hi_(hi) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
        return lo_ + hi_ * (1.0 - std::cos(std::numbers::pi * u)) * 0.5;
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
