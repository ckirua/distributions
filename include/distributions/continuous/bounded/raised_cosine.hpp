#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <numbers>
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct RaisedCosineDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double lo_;
    double hi_;
    RaisedCosineDistribution(double lo, double hi) : lo_(lo), hi_(hi) {
        detail::assert_double_interval(lo_, hi_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
                return static_cast<Sample>(lo_ + hi_ * (1.0 - std::cos(std::numbers::pi * u)) * 0.5);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using RaisedCosine = RaisedCosineDistribution<double>;

}  // namespace distributions
