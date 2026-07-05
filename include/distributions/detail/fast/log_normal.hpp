#pragma once

#include "distributions/detail/fast/normal.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void log_normal_sample_batch(
    double* out, std::size_t n, double mu, double sigma, std::uint64_t seed) {
    normal_sample_batch(out, n, mu, sigma, seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = std::exp(out[i]);
    }
}

}  // namespace distributions::detail::fast
