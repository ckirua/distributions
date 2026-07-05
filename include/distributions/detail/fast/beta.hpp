#pragma once

#include "distributions/detail/fast/gamma.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void beta_sample_batch(
    double* out, std::size_t n, double alpha, double beta, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    int ka = 0;
    int kb = 0;
    if (integer_shape(alpha, ka) && integer_shape(beta, kb)) {
        for (std::size_t i = 0; i < n; ++i) {
            const double x = erlang_sum(rng, ka);
            const double y = erlang_sum(rng, kb);
            out[i] = x / (x + y);
        }
        return;
    }
    for (std::size_t i = 0; i < n; ++i) {
        const double x = sample_gamma_splitmix(rng, alpha, 1.0);
        const double y = sample_gamma_splitmix(rng, beta, 1.0);
        out[i] = x / (x + y);
    }
}

}  // namespace distributions::detail::fast
