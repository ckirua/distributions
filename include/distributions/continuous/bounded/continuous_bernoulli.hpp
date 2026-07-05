#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/continuous_bernoulli.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct ContinuousBernoulli {
    double lambda_;
    ContinuousBernoulli(double lambda) : lambda_(lambda) {
        detail::assert_nonnegative(lambda_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_continuous_bernoulli(rng, lambda_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        if (n >= detail::kFastThreshold) {
            detail::fast::continuous_bernoulli_sample_batch(out, n, lambda_, detail::batch_seed_from(rng));
            return;
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
