#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/detail/hypergeometric.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NegativeHypergeometric {
    int M_;
    int n_;
    int N_;
    NegativeHypergeometric(int M, int n, int N) : M_(M), n_(n), N_(N) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        const int k = detail::sample_hypergeometric(rng, M_, n_, N_);
        return detail::sample_negative_binomial(rng, static_cast<double>(k), static_cast<double>(N_) / static_cast<double>(M_));
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
