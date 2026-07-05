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
struct WignerSemicircleDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double lo_;
    double scale_;
    WignerSemicircleDistribution(double lo, double scale) : lo_(lo), scale_(scale) {
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
                const double r = std::sqrt(u) * scale_ * 0.5;
                return static_cast<Sample>(lo_ + r);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using WignerSemicircle = WignerSemicircleDistribution<double>;

}  // namespace distributions
