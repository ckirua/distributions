#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct GeneralizedHyperbolic {
    double p_;
    double a_;
    double b_;
    GeneralizedHyperbolic(double p, double a, double b) : p_(p), a_(a), b_(b) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_genhyperbolic(rng, p_, a_, b_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
