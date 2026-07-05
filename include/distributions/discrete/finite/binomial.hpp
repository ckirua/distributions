#pragma once

#include "distributions/base.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>

namespace distributions {

struct Binomial : DistributionBase<Binomial, int, Pcg32> {
    int n;
    double p;

    Binomial(int n = 1, double p = 0.5) : n(n), p(p) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        // Inversion method (fixed n).
        double cumulative = 0.0;
        const double q = std::pow(1.0 - p, static_cast<double>(n));
        double prob = q;
        const double u = rng.next_double();
        cumulative = prob;
        if (u <= cumulative) {
            return 0;
        }
        for (int k = 1; k <= n; ++k) {
            prob *= (static_cast<double>(n - k + 1) / k) * (p / (1.0 - p));
            cumulative += prob;
            if (u <= cumulative) {
                return k;
            }
        }
        return n;
    }

    void sample_batch(int* out, std::size_t n_out, Pcg32& rng) const {
        for (std::size_t i = 0; i < n_out; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return static_cast<double>(n) * p; }

    [[nodiscard]] double variance() const { return static_cast<double>(n) * p * (1.0 - p); }
};

}  // namespace distributions
