#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct WignerSemicircle {
    double lo_;
    double scale_;
    WignerSemicircle(double lo, double scale) : lo_(lo), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
        const double r = std::sqrt(u) * scale_ * 0.5;
        return lo_ + r;
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
