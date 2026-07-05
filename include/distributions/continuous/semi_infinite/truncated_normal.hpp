#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct TruncatedNormal {
    double a_;
    double b_;
    double loc_;
    double scale_;
    TruncatedNormal(double a, double b, double loc, double scale) : a_(a), b_(b), loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        for (;;) {
            const double x = detail::sample_normal(rng, loc_, scale_);
            if (x >= a_ && x <= b_) { return x; }
        }
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
