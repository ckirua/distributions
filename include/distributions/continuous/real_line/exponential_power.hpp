#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ExponentialPowerDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double scale_;
    ExponentialPowerDistribution(double scale) : scale_(scale) {
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(-std::log1p(-rng.next_double()) * scale_);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using ExponentialPower = ExponentialPowerDistribution<double>;

}  // namespace distributions
