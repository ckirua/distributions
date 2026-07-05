#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct GeneralizedChiSquaredDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double df1_;
    double w1_;
    double df2_;
    double w2_;
    GeneralizedChiSquaredDistribution(double df1, double w1, double df2, double w2) : df1_(df1), w1_(w1), df2_(df2), w2_(w2) {
        detail::assert_finite(df1_);
        detail::assert_probability(w1_);
        detail::assert_finite(df2_);
        detail::assert_finite(w2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_generalized_chi_squared(rng, df1_, w1_, df2_, w2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using GeneralizedChiSquared = GeneralizedChiSquaredDistribution<double>;

}  // namespace distributions
