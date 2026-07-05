#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NoncentralBeta {
    double alpha_;
    double beta_;
    double lam_;
    NoncentralBeta(double alpha, double beta, double lam) : alpha_(alpha), beta_(beta), lam_(lam) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_noncentral_beta(rng, alpha_, beta_, lam_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
