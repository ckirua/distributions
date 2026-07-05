#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/beta_prime.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct BetaPrimeDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double alpha_;
    double beta_;
    BetaPrimeDistribution(double alpha, double beta) : alpha_(alpha), beta_(beta) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double x = detail::sample_gamma(rng, alpha_, 1.0);
                const double y = detail::sample_gamma(rng, beta_, 1.0);
                return static_cast<Sample>(x / y);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, double>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::beta_prime_sample_batch(out, n, alpha_, beta_, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using BetaPrime = BetaPrimeDistribution<double>;

}  // namespace distributions
