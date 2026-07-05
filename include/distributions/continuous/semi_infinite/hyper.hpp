#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Hyper {
    double lambda1_;
    double lambda2_;
    double w1_;
    Hyper(double lambda1, double lambda2, double w1) : lambda1_(lambda1), lambda2_(lambda2), w1_(w1) {
        detail::assert_nonnegative(lambda1_);
        detail::assert_nonnegative(lambda2_);
        detail::assert_probability(w1_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_hyper_mixture(rng, lambda1_, lambda2_, w1_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
