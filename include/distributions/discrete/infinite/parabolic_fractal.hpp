#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct ParabolicFractalDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double b_;
    double c_;
    ParabolicFractalDistribution(double b, double c) : b_(b), c_(c) {
        detail::assert_strictly_positive(b_);
        detail::assert_strictly_positive(c_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_parabolic_fractal(rng, b_, c_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using ParabolicFractal = ParabolicFractalDistribution<int>;

}  // namespace distributions
