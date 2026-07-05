#pragma once

#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/beta.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Beta {
    double alpha_;
    double beta_;
    Beta(double alpha, double beta) : alpha_(alpha), beta_(beta) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_beta(rng, alpha_, beta_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        if (n >= detail::kFastThreshold) {
            detail::fast::beta_sample_batch(out, n, alpha_, beta_, detail::batch_seed_from(rng));
            return;
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
