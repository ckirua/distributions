#pragma once

#include "distributions/detail/validate.hpp"
#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct GeneralizedChiSquared {
    double df1_;
    double w1_;
    double df2_;
    double w2_;
    GeneralizedChiSquared(double df1, double w1, double df2, double w2) : df1_(df1), w1_(w1), df2_(df2), w2_(w2) {
        detail::assert_finite(df1_);
        detail::assert_probability(w1_);
        detail::assert_finite(df2_);
        detail::assert_finite(w2_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_generalized_chi_squared(rng, df1_, w1_, df2_, w2_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
