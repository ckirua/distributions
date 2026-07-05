#pragma once

#include "distributions/detail/fast/gamma.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void chi_squared_sample_batch(
    double* out, std::size_t n, double df, std::uint64_t seed) {
    gamma_sample_batch(out, n, df * 0.5, 2.0, seed);
}

inline void chi_sample_batch(double* out, std::size_t n, double df, std::uint64_t seed) {
    chi_squared_sample_batch(out, n, df, seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = std::sqrt(out[i]);
    }
}

inline void inverse_chi_squared_sample_batch(
    double* out, std::size_t n, double df, double scale, std::uint64_t seed) {
    chi_squared_sample_batch(out, n, df, seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = scale / out[i];
    }
}

}  // namespace distributions::detail::fast
