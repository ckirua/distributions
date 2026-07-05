#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MultivariateStableDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double alpha_;
    double beta_;
    MultivariateStableDistribution(double alpha, double beta) : alpha_(alpha), beta_(beta) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_multivariate_stable_first(rng, alpha_, beta_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using MultivariateStable = MultivariateStableDistribution<double>;

}  // namespace distributions
