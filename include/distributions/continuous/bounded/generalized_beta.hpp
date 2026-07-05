#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct GeneralizedBetaDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double alpha_;
    double beta_;
    double lambda_;
    GeneralizedBetaDistribution(double alpha, double beta, double lambda) : alpha_(alpha), beta_(beta), lambda_(lambda) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
        detail::assert_nonnegative(lambda_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_generalized_beta(rng, alpha_, beta_, lambda_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using GeneralizedBeta = GeneralizedBetaDistribution<double>;

}  // namespace distributions
