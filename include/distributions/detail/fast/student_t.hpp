#pragma once

#include "distributions/detail/fast/gamma.hpp"
#include "distributions/detail/fast/normal.hpp"
#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void student_t_sample_batch(
    double* out, std::size_t n, double df, std::uint64_t seed) {
    normal_sample_batch(out, n, 0.0, 1.0, seed);
    SplitMix64Stream rng;
    rng.seed(seed ^ 0x9e3779b97f4a7c15ULL);
    const double half_df = df * 0.5;
    for (std::size_t i = 0; i < n; ++i) {
        const double v = sample_gamma_splitmix(rng, half_df, 2.0);
        out[i] *= std::sqrt(df / v);
    }
}

}  // namespace distributions::detail::fast
