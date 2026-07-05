#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/bounded.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/continuous_bernoulli.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ContinuousBernoulliDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double lambda_;
    ContinuousBernoulliDistribution(double lambda) : lambda_(lambda) {
        detail::assert_nonnegative(lambda_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_continuous_bernoulli(rng, lambda_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, double>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::continuous_bernoulli_sample_batch(out, n, lambda_, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using ContinuousBernoulli = ContinuousBernoulliDistribution<double>;

}  // namespace distributions
