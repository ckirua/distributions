#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct BetaNegativeBinomialDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double alpha_;
    double beta_;
    double r_;
    BetaNegativeBinomialDistribution(double alpha, double beta, double r) : alpha_(alpha), beta_(beta), r_(r) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
        detail::assert_strictly_positive(r_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_beta_negative_binomial(rng, alpha_, beta_, r_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using BetaNegativeBinomial = BetaNegativeBinomialDistribution<int>;

}  // namespace distributions
