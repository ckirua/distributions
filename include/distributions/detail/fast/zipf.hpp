#pragma once

#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

inline int zipf_cdf_sample(Pcg32& rng, int n, const double* cdf, double cdf_n) {
    const double u = rng.next_double() * cdf_n;
    int lo = 1;
    int hi = n;
    while (lo < hi) {
        const int mid = (lo + hi) / 2;
        if (cdf[mid] < u) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

inline void zipf_sample_batch(
    int* out, std::size_t n, int rank_n, const double* cdf, double cdf_n, std::uint64_t seed) {
    Pcg32 rng(seed);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = zipf_cdf_sample(rng, rank_n, cdf, cdf_n);
    }
}

}  // namespace distributions::detail::fast
