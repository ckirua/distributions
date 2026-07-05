#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct KaniadakisWeibullDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double kappa_;
    double shape_;
    double scale_;
    KaniadakisWeibullDistribution(double kappa, double shape, double scale) : kappa_(kappa), shape_(shape), scale_(scale) {
        detail::assert_nonnegative(kappa_);
        detail::assert_strictly_positive(shape_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_kaniadakis_weibull(rng, kappa_, shape_, scale_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using KaniadakisWeibull = KaniadakisWeibullDistribution<double>;

}  // namespace distributions
