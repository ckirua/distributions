#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MarchenkopasturDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double gamma_ratio_;
    double sigma_;
    MarchenkopasturDistribution(double gamma_ratio, double sigma) : gamma_ratio_(gamma_ratio), sigma_(sigma) {
        detail::assert_finite(gamma_ratio_);
        detail::assert_strictly_positive(sigma_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_marchenko_pastur(rng, gamma_ratio_, sigma_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Marchenkopastur = MarchenkopasturDistribution<double>;

}  // namespace distributions
