#pragma once

#include "distributions/detail/fast/gamma.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void beta_prime_sample_batch(
    double* out, std::size_t n, double alpha, double beta, std::uint64_t seed) {
    static constexpr std::size_t kBlock = 256;
    double x[kBlock];
    double y[kBlock];
    std::size_t i = 0;
    std::uint64_t chunk_seed = seed;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        gamma_sample_batch(x, chunk, alpha, 1.0, chunk_seed);
        gamma_sample_batch(y, chunk, beta, 1.0, chunk_seed ^ 0x9e3779b97f4a7c15ULL);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] = x[j] / y[j];
        }
        i += chunk;
        chunk_seed += 0x517cc1b727220a95ULL;
    }
}

}  // namespace distributions::detail::fast
