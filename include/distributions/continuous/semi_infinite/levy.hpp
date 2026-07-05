#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct LevyDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double loc_;
    double scale_;
    LevyDistribution(double loc, double scale) : loc_(loc), scale_(scale) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double z = detail::sample_standard_normal(rng);
                return static_cast<Sample>(loc_ + scale_ / (z * z));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Levy = LevyDistribution<double>;

}  // namespace distributions
