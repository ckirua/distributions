#pragma once

#include "distributions/detail/multivariate_discrete.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Multinomial {
    int n_;
    double p0_;
    Multinomial(int n, double p0) : n_(n), p0_(p0) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_multinomial_first(rng, n_, p0_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
