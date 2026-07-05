#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"
#include "distributions/detail/math.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void geometric_sample_batch(int* out, std::size_t n, double p, std::uint64_t seed) {
    const double denom = detail::log1p_neg(p);
    SplitMix64Stream rng;
    rng.seed(seed);
    constexpr std::size_t kBlock = 256;
    double uniforms[kBlock];
    std::size_t i = 0;
    while (i < n) {
        const std::size_t chunk = std::min(kBlock, n - i);
        rng.fill_uniform01(uniforms, chunk);
        for (std::size_t j = 0; j < chunk; ++j) {
            out[i + j] =
                static_cast<int>(std::floor(std::log1p(-uniforms[j]) / denom)) + 1;
        }
        i += chunk;
    }
}

}  // namespace distributions::detail::fast
