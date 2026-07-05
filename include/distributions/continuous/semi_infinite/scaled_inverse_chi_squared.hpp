#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ScaledInverseChiSquaredDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double df_;
    double scale_;
    ScaledInverseChiSquaredDistribution(double df, double scale) : df_(df), scale_(scale) {
        detail::assert_strictly_positive(df_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(scale_ / detail::sample_chi_squared(rng, df_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using ScaledInverseChiSquared = ScaledInverseChiSquaredDistribution<double>;

}  // namespace distributions
