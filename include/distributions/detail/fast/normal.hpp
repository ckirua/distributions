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

/// Tier B for ``float`` output (compute in ``double``, store as ``float``).
inline void normal_sample_batch(
    float* out, std::size_t n, double mu, double sigma, std::uint64_t seed) {
    static constexpr float kMinU1 = 1e-7f;
    SplitMix64Stream rng;
    rng.seed(seed);
    static constexpr double kTwoPi = 6.283185307179586;
    std::size_t i = 0;
    while (i < n) {
        const float u1 = std::max(static_cast<float>(rng.next_double()), kMinU1);
        const float u2 = static_cast<float>(rng.next_double());
        const float r = std::sqrt(-2.0f * std::log(u1));
        const float theta = static_cast<float>(kTwoPi) * u2;
        out[i++] = static_cast<float>(mu + sigma * r * std::cos(theta));
        if (i < n) {
            out[i++] = static_cast<float>(mu + sigma * r * std::sin(theta));
        }
    }
}

}  // namespace distributions::detail::fast
