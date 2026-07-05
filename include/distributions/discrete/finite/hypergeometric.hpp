#pragma once

#include "distributions/detail/hypergeometric.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Hypergeometric {
    int M_;
    int n_success_;
    int N_draws_;
    Hypergeometric(int M, int n_success, int N_draws) : M_(M), n_success_(n_success), N_draws_(N_draws) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_hypergeometric(rng, M_, n_success_, N_draws_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
