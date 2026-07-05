#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/matrix.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct UniformDistributionOnAStiefelManifoldDistribution {
    static_assert(is_continuous_sample_v<Sample>);


    UniformDistributionOnAStiefelManifoldDistribution() = default;

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_stiefel_element(rng));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using UniformDistributionOnAStiefelManifold = UniformDistributionOnAStiefelManifoldDistribution<double>;

}  // namespace distributions
