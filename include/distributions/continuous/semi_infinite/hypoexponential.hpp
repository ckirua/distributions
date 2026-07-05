#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct HypoexponentialDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double rate1_;
    double rate2_;
    HypoexponentialDistribution(double rate1, double rate2) : rate1_(rate1), rate2_(rate2) {
        detail::assert_finite(rate1_);
        detail::assert_finite(rate2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_hypoexponential(rng, rate1_, rate2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Hypoexponential = HypoexponentialDistribution<double>;

}  // namespace distributions
