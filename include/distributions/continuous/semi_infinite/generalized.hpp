#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Generalized {
    double a_;
    double c_;
    double scale_;
    Generalized(double a, double c, double scale) : a_(a), c_(c), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return scale_ * std::pow(detail::sample_gamma(rng, a_, 1.0), 1.0 / c_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
