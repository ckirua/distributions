#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MultivariateLaplaceDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double scale_;
    MultivariateLaplaceDistribution(double scale) : scale_(scale) {
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_multivariate_laplace_first(rng, scale_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using MultivariateLaplace = MultivariateLaplaceDistribution<double>;

}  // namespace distributions
