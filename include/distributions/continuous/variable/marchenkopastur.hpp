#pragma once

#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Marchenkopastur {
    double gamma_ratio_;
    double sigma_;
    Marchenkopastur(double gamma_ratio, double sigma) : gamma_ratio_(gamma_ratio), sigma_(sigma) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_marchenko_pastur(rng, gamma_ratio_, sigma_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
