#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct FoldedNormalDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double c_;
    double loc_;
    double scale_;
    FoldedNormalDistribution(double c, double loc, double scale) : c_(c), loc_(loc), scale_(scale) {
        detail::assert_strictly_positive(c_);
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        for (;;) {
                    const double x = loc_ + scale_ * detail::sample_standard_normal(rng);
                    if (x >= c_) { return static_cast<Sample>(x); }
                }
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using FoldedNormal = FoldedNormalDistribution<double>;

}  // namespace distributions
