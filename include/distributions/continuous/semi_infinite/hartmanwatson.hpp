#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct HartmanwatsonDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double nu_;
    HartmanwatsonDistribution(double nu) : nu_(nu) {
        detail::assert_strictly_positive(nu_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_hartman_watson(rng, nu_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Hartmanwatson = HartmanwatsonDistribution<double>;

}  // namespace distributions
