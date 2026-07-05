#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <numbers>

namespace distributions::detail::fast {

inline double sample_gamma_splitmix(SplitMix64Stream& rng, double shape, double scale) {
    if (shape <= 0.0 || scale <= 0.0) {
        return 0.0;
    }
    if (shape < 1.0) {
        const double u = rng.next_double();
        return sample_gamma_splitmix(rng, shape + 1.0, scale) * std::pow(u, 1.0 / shape);
    }
    const double d = shape - 1.0 / 3.0;
    const double c = 1.0 / std::sqrt(9.0 * d);
    for (;;) {
        double x = 0.0;
        double v = 0.0;
        do {
            const double u1 = rng.next_double();
            const double u2 = rng.next_double();
            x = std::sqrt(-2.0 * std::log(u1 + 1e-300)) *
                std::cos(2.0 * std::numbers::pi * u2);
            v = 1.0 + c * x;
        } while (v <= 0.0);
        v = v * v * v;
        const double u = rng.next_double();
        if (u < 1.0 - 0.0331 * (x * x) * (x * x)) {
            return d * v * scale;
        }
        if (std::log(u) < 0.5 * x * x + d * (1.0 - v + std::log(v))) {
            return d * v * scale;
        }
    }
}

inline bool integer_shape(double shape, int& k) {
    k = static_cast<int>(std::lround(shape));
    return k >= 1 && k <= 64 && std::fabs(shape - static_cast<double>(k)) < 1e-9;
}

inline double erlang_sum(SplitMix64Stream& rng, int shape) {
    double sum = 0.0;
    for (int j = 0; j < shape; ++j) {
        sum -= std::log(std::max(rng.next_double(), 1e-300));
    }
    return sum;
}

inline void gamma_sample_batch(
    double* out, std::size_t n, double shape, double scale, std::uint64_t seed) {
    SplitMix64Stream rng;
    rng.seed(seed);
    int k = 0;
    if (integer_shape(shape, k)) {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = scale * erlang_sum(rng, k);
        }
        return;
    }
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = sample_gamma_splitmix(rng, shape, scale);
    }
}

}  // namespace distributions::detail::fast
