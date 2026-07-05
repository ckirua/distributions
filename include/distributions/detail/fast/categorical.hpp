#pragma once

#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline int categorical_sample(Pcg32& rng, int k, const int* alias, const double* prob) {
    const int col = static_cast<int>(rng.next_u32() % static_cast<std::uint32_t>(k));
    const double u = rng.next_double();
    return u < prob[col] ? col : alias[col];
}

inline void categorical_sample_batch(
    int* out,
    std::size_t n,
    int k,
    const int* alias,
    const double* prob,
    std::uint64_t seed) {
    Pcg32 rng(seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = categorical_sample(rng, k, alias, prob);
    }
}

}  // namespace distributions::detail::fast
