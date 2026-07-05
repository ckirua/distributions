#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Delaporte {
    double lambda_;
    double r_;
    double p_;
    Delaporte(double lambda, double r, double p) : lambda_(lambda), r_(r), p_(p) {
        detail::assert_nonnegative(lambda_);
        detail::assert_strictly_positive(r_);
        detail::assert_probability(p_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_delaporte(rng, lambda_, r_, p_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
