#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ShiftedDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double loc_;
    double rate_;
    ShiftedDistribution(double loc, double rate) : loc_(loc), rate_(rate) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(rate_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_shifted_exponential(rng, loc_, rate_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Shifted = ShiftedDistribution<double>;

}  // namespace distributions
