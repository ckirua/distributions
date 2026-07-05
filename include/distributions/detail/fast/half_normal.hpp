#pragma once

#include "distributions/detail/fast/normal.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void half_normal_sample_batch(
    double* out, std::size_t n, double loc, double scale, std::uint64_t seed) {
    normal_sample_batch(out, n, 0.0, 1.0, seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = loc + scale * std::abs(out[i]);
    }
}

}  // namespace distributions::detail::fast
