#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct NoncentralChiSquaredDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double df_;
    double ncp_;
    NoncentralChiSquaredDistribution(double df, double ncp) : df_(df), ncp_(ncp) {
        detail::assert_strictly_positive(df_);
        detail::assert_nonnegative(ncp_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const int j = detail::sample_poisson(rng, ncp_ * 0.5);
                return static_cast<Sample>(detail::sample_chi_squared(rng, df_ + 2.0 * static_cast<double>(j)));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using NoncentralChiSquared = NoncentralChiSquaredDistribution<double>;

}  // namespace distributions
