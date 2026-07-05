#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void normal_sample_batch(
    double* out, std::size_t n, double mu, double sigma, std::uint64_t seed) {
    static constexpr double kTwoPi = 6.283185307179586;
    SplitMix64Stream rng;
    rng.seed(seed);
    std::size_t i = 0;
    while (i < n) {
        const double u1 = std::max(rng.next_double(), 1e-300);
        const double u2 = rng.next_double();
        const double r = std::sqrt(-2.0 * std::log(u1));
        const double theta = kTwoPi * u2;
        out[i++] = mu + sigma * r * std::cos(theta);
        if (i < n) {
            out[i++] = mu + sigma * r * std::sin(theta);
        }
    }
}

}  // namespace distributions::detail::fast
