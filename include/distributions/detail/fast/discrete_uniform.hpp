#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void discrete_uniform_sample_batch(
    int* out, std::size_t n, int low, int high, std::uint64_t seed) {
    const double scale = static_cast<double>(high - low + 1);
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = low + static_cast<int>(rng.next_double() * scale);
    }
}

}  // namespace distributions::detail::fast
