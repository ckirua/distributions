#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct DiscretePhaseTypeDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double p1_;
    double p2_;
    DiscretePhaseTypeDistribution(double p1, double p2) : p1_(p1), p2_(p2) {
        detail::assert_probability(p1_);
        detail::assert_probability(p2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_discrete_phase_type(rng, p1_, p2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using DiscretePhaseType = DiscretePhaseTypeDistribution<int>;

}  // namespace distributions
