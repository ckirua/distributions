#pragma once

#include "distributions/detail/poisson.hpp"
#include "distributions/rng.hpp"

#include <cstddef>

namespace distributions {

struct Skellam {
    double mu1;
    double mu2;

    Skellam(double mu1 = 1.0, double mu2 = 1.0) : mu1(mu1), mu2(mu2) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_poisson(rng, mu1) - detail::sample_poisson(rng, mu2);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return mu1 - mu2; }

    [[nodiscard]] double variance() const { return mu1 + mu2; }
};

}  // namespace distributions
