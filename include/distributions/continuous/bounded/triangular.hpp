#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Triangular {
    double lo_;
    double mode_;
    double hi_;
    Triangular(double lo, double mode, double hi) : lo_(lo), mode_(mode), hi_(hi) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = rng.next_double();
        const double c = (mode_ - lo_) / (hi_ - lo_);
        if (u < c) { return lo_ + std::sqrt(u * (hi_ - lo_) * (mode_ - lo_)); }
        return hi_ - std::sqrt((1.0 - u) * (hi_ - lo_) * (hi_ - mode_));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
