#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline int sample_poisson_splitmix(SplitMix64Stream& rng, double lambda) {
    if (lambda <= 0.0) {
        return 0;
    }
    if (lambda > 30.0) {
        const double x = lambda + std::sqrt(lambda) * (rng.next_double() * 2.0 - 1.0) * 3.0;
        return x < 0.0 ? 0 : static_cast<int>(std::lround(x));
    }
    const double limit = std::exp(-lambda);
    double p = 1.0;
    int k = 0;
    do {
        ++k;
        p *= rng.next_double();
    } while (p > limit);
    return k - 1;
}

inline void poisson_sample_batch(int* out, std::size_t n, double lambda, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = sample_poisson_splitmix(rng, lambda);
    }
}

}  // namespace distributions::detail::fast
