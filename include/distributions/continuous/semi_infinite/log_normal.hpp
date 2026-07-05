#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct LogNormal {
    double mu_;
    double sigma_;
    LogNormal(double mu, double sigma) : mu_(mu), sigma_(sigma) {
        detail::assert_finite(mu_);
        detail::assert_strictly_positive(sigma_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return std::exp(detail::sample_normal(rng, mu_, sigma_));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
