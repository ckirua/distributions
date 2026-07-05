#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/circular.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct WrappedExponentialDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double loc_;
    double rate_;
    WrappedExponentialDistribution(double loc, double rate) : loc_(loc), rate_(rate) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(rate_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = rng.next_double();
                return static_cast<Sample>(detail::wrap_angle(loc_ - std::log1p(-u) / rate_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using WrappedExponential = WrappedExponentialDistribution<double>;

}  // namespace distributions
