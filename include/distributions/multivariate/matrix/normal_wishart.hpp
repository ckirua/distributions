#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct NormalWishartDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double df_;
    double kappa_;
    double v00_;
    double v11_;
    NormalWishartDistribution(double df, double kappa, double v00, double v11) : df_(df), kappa_(kappa), v00_(v00), v11_(v11) {
        detail::assert_strictly_positive(df_);
        detail::assert_nonnegative(kappa_);
        detail::assert_finite(v00_);
        detail::assert_finite(v11_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_normal_wishart_mean(rng, df_, kappa_, v00_, v11_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using NormalWishart = NormalWishartDistribution<double>;

}  // namespace distributions
