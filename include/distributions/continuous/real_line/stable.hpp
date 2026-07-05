#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/real_line.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct StableDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double alpha_;
    double beta_;
    double loc_;
    double scale_;
    StableDistribution(double alpha, double beta, double loc, double scale) : alpha_(alpha), beta_(beta), loc_(loc), scale_(scale) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_stable(rng, alpha_, beta_, loc_, scale_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Stable = StableDistribution<double>;

}  // namespace distributions
