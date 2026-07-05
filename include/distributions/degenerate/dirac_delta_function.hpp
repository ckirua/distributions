#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct DiracDeltaDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double x0_;
    DiracDeltaDistribution(double x0) : x0_(x0) {
        detail::assert_finite(x0_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(x0_);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using DiracDelta = DiracDeltaDistribution<double>;

}  // namespace distributions
