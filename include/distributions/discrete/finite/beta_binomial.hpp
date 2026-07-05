#pragma once

#include "distributions/discrete/finite/binomial.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/beta_binomial.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/simd/beta_binomial.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/rng.hpp"

#include <cstddef>

namespace distributions {

struct BetaBinomial {
    int n;
    double alpha;
    double beta;

    BetaBinomial(int n = 1, double alpha = 1.0, double beta = 1.0)
        : n(n), alpha(alpha), beta(beta) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        const double p = detail::sample_beta(rng, alpha, beta);
        const Binomial binom(n, p);
        return binom.sample(rng);
    }

    void sample_batch(int* out, std::size_t n_out, Pcg32& rng) const {
        if (n_out >= detail::kFastThreshold && n <= detail::fast::kMaxBernoulliSumTrials) {
            detail::simd::beta_binomial_sample_batch(
                out, n_out, n, alpha, beta, detail::batch_seed_from(rng));
            return;
        }
        for (std::size_t i = 0; i < n_out; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const {
        return static_cast<double>(n) * alpha / (alpha + beta);
    }

    [[nodiscard]] double variance() const {
        const double ab = alpha + beta;
        return static_cast<double>(n) * alpha * beta * (ab + static_cast<double>(n)) /
               (ab * ab * (ab + 1.0));
    }
};

}  // namespace distributions
