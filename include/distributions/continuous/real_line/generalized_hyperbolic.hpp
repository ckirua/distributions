#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/real_line.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct GeneralizedHyperbolicDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double p_;
    double a_;
    double b_;
    GeneralizedHyperbolicDistribution(double p, double a, double b) : p_(p), a_(a), b_(b) {
        detail::assert_finite(p_);
        detail::assert_gh_support(a_, b_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_genhyperbolic(rng, p_, a_, b_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using GeneralizedHyperbolic = GeneralizedHyperbolicDistribution<double>;

}  // namespace distributions
