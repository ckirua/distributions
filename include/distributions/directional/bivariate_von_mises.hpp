#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct BivariateVonMisesDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double kappa1_;
    double kappa2_;
    BivariateVonMisesDistribution(double kappa1, double kappa2) : kappa1_(kappa1), kappa2_(kappa2) {
        detail::assert_nonnegative(kappa1_);
        detail::assert_nonnegative(kappa2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_bivariate_von_mises_first(rng, kappa1_, kappa2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using BivariateVonMises = BivariateVonMisesDistribution<double>;

}  // namespace distributions
