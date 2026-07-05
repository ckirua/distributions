#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct SkewNormalDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double loc_;
    double scale_;
    double alpha_;
    SkewNormalDistribution(double loc, double scale, double alpha) : loc_(loc), scale_(scale), alpha_(alpha) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
        detail::assert_strictly_positive(alpha_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u1 = detail::sample_standard_normal(rng);
                const double u2 = detail::sample_standard_normal(rng);
                const double delta = alpha_ / std::sqrt(1.0 + alpha_ * alpha_);
                return static_cast<Sample>(loc_ + scale_ * (delta * std::abs(u1) + std::sqrt(1.0 - delta * delta) * u2));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using SkewNormal = SkewNormalDistribution<double>;

}  // namespace distributions
