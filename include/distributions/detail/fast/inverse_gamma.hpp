#pragma once

#include "distributions/detail/fast/gamma.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void inverse_gamma_sample_batch(
    double* out, std::size_t n, double shape, double scale, std::uint64_t seed) {
    gamma_sample_batch(out, n, shape, 1.0, seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = scale / out[i];
    }
}

}  // namespace distributions::detail::fast
