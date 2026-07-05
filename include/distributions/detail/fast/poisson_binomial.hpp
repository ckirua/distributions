#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void poisson_binomial_sample_batch(
    int* out,
    std::size_t n,
    const double* probs,
    std::size_t k,
    std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        int sum = 0;
        for (std::size_t j = 0; j < k; ++j) {
            sum += rng.next_double() < probs[j] ? 1 : 0;
        }
        out[i] = sum;
    }
}

}  // namespace distributions::detail::fast
