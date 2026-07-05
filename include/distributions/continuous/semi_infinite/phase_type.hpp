#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct PhaseTypeDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double rate1_;
    double rate2_;
    double rate3_;
    PhaseTypeDistribution(double rate1, double rate2, double rate3) : rate1_(rate1), rate2_(rate2), rate3_(rate3) {
        detail::assert_finite(rate1_);
        detail::assert_finite(rate2_);
        detail::assert_finite(rate3_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_phase_type(rng, rate1_, rate2_, rate3_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using PhaseType = PhaseTypeDistribution<double>;

}  // namespace distributions
