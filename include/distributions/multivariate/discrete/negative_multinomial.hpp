#pragma once

#include "distributions/detail/multivariate_discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NegativeMultinomial {
    double r_;
    double p0_;
    double p1_;
    double p2_;
    NegativeMultinomial(double r, double p0, double p1, double p2) : r_(r), p0_(p0), p1_(p1), p2_(p2) {
        detail::assert_strictly_positive(r_);
        detail::assert_finite(p0_);
        detail::assert_probability(p1_);
        detail::assert_probability(p2_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_negative_multinomial_first(rng, r_, p0_, p1_, p2_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
