#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ArcsineDistribution {
    static_assert(is_continuous_sample_v<Sample>);


    ArcsineDistribution() = default;

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_beta(rng, 0.5, 0.5));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Arcsine = ArcsineDistribution<double>;

}  // namespace distributions
