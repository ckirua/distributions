#pragma once

#include "distributions/detail/poisson.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline void skellam_sample_batch(
    int* out, std::size_t n, double mu1, double mu2, std::uint64_t seed) {
    Pcg32 rng(seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = detail::sample_poisson(rng, mu1) - detail::sample_poisson(rng, mu2);
    }
}

}  // namespace distributions::detail::fast
