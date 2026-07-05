#pragma once

#include "distributions/detail/fast/gamma.hpp"
#include "distributions/detail/fast/normal.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void student_t_sample_batch(
    double* out, std::size_t n, double df, std::uint64_t seed) {
    normal_sample_batch(out, n, 0.0, 1.0, seed);
    static constexpr std::size_t kBlock = 256;
    double gamma_buf[kBlock];
    const double half_df = df * 0.5;
    const double scale = std::sqrt(df);
    std::uint64_t chunk_seed = seed ^ 0x9e3779b97f4a7c15ULL;
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        gamma_sample_batch(gamma_buf, chunk, half_df, 2.0, chunk_seed);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] *= scale / std::sqrt(gamma_buf[j]);
        }
        i += chunk;
        chunk_seed += 0x517cc1b727220a95ULL;
    }
}

}  // namespace distributions::detail::fast
