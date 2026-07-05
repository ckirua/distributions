#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct ExtendedNegativeBinomial {
    double r_;
    double p_;
    ExtendedNegativeBinomial(double r, double p) : r_(r), p_(p) {
        detail::assert_strictly_positive(r_);
        detail::assert_open_unit_interval(p_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_negative_binomial_gamma_poisson(rng, r_, p_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
