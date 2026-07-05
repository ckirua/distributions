#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/rng.hpp"

#include <cstddef>

namespace distributions {

struct NegativeBinomial {
    int r;
    double p;

    NegativeBinomial(int r = 1, double p = 0.5) : r(r), p(p) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        const double lambda = detail::sample_gamma(rng, static_cast<double>(r), 1.0);
        const double rate = (1.0 - p) / p;
        return detail::sample_poisson(rng, lambda * rate);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return static_cast<double>(r) * (1.0 - p) / p; }

    [[nodiscard]] double variance() const {
        return static_cast<double>(r) * (1.0 - p) / (p * p);
    }
};

}  // namespace distributions
