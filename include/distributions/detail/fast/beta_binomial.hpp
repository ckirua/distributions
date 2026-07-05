#pragma once

#include "distributions/detail/fast/binomial.hpp"
#include "distributions/detail/fast/splitmix_stream.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void beta_binomial_sample_batch(
    int* out, std::size_t n, int trials, double alpha, double beta, std::uint64_t seed) {
    SplitMix64Stream sm;
    sm.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        Pcg32 beta_rng(sm.next_u64());
        const double p = detail::sample_beta(beta_rng, alpha, beta);
        out[i] = binomial_bernoulli_sum(sm, trials, p);
    }
}

}  // namespace distributions::detail::fast
