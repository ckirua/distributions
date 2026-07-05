#pragma once

#include "distributions/detail/inverse_gaussian.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct InverseGaussian {
    double mu_;
    double lambda_;
    InverseGaussian(double mu, double lambda) : mu_(mu), lambda_(lambda) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_inverse_gaussian(rng, mu_, lambda_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
