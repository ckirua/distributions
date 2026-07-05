#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void bernoulli_sample_batch(int* out, std::size_t n, double p, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    const std::uint64_t threshold =
        static_cast<std::uint64_t>(p * static_cast<double>(1ULL << 53));
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = (rng.next_u64() >> 11U) < threshold ? 1 : 0;
    }
}

}  // namespace distributions::detail::fast
