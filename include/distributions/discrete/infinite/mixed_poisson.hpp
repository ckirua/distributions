#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct MixedPoissonDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double shape_;
    double scale_;
    MixedPoissonDistribution(double shape, double scale) : shape_(shape), scale_(scale) {
        detail::assert_strictly_positive(shape_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_mixed_poisson(rng, shape_, scale_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using MixedPoisson = MixedPoissonDistribution<int>;

}  // namespace distributions
