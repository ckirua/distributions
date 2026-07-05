#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct BetaRectangularDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double alpha_;
    double beta_;
    BetaRectangularDistribution(double alpha, double beta) : alpha_(alpha), beta_(beta) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_beta(rng, alpha_, beta_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using BetaRectangular = BetaRectangularDistribution<double>;

}  // namespace distributions
