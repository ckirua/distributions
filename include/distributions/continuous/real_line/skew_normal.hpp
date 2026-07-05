#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct SkewNormal {
    double loc_;
    double scale_;
    double alpha_;
    SkewNormal(double loc, double scale, double alpha) : loc_(loc), scale_(scale), alpha_(alpha) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u1 = detail::sample_standard_normal(rng);
        const double u2 = detail::sample_standard_normal(rng);
        const double delta = alpha_ / std::sqrt(1.0 + alpha_ * alpha_);
        return loc_ + scale_ * (delta * std::abs(u1) + std::sqrt(1.0 - delta * delta) * u2);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
