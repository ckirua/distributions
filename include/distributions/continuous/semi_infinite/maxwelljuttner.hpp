#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MaxwelljuttnerDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    MaxwelljuttnerDistribution(double a) : a_(a) {
        detail::assert_strictly_positive(a_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_maxwell_juttner(rng, a_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Maxwelljuttner = MaxwelljuttnerDistribution<double>;

}  // namespace distributions
