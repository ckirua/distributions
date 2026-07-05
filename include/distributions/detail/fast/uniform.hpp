#pragma once

#include "distributions/detail/counter_rng.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

/// Tier-B uniform fill on [0, 1) using `CounterRng` (single-thread).
inline void fill_uniform01(double* out, std::size_t n, std::uint64_t seed, std::uint32_t stream = 0) {
    CounterRng rng;
    rng.seed(seed, stream);
    rng.fill_uniform01(out, n);
}

}  // namespace distributions::detail::fast
