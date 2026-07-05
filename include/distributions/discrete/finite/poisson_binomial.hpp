#pragma once

#include "distributions/rng.hpp"

#include <cstddef>
#include <vector>

namespace distributions {

struct PoissonBinomial {
    std::vector<double> probs;

    explicit PoissonBinomial(std::vector<double> probs) : probs(std::move(probs)) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        int sum = 0;
        for (double p : probs) {
            if (rng.next_double() < p) {
                ++sum;
            }
        }
        return sum;
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const {
        double m = 0.0;
        for (double p : probs) {
            m += p;
        }
        return m;
    }

    [[nodiscard]] double variance() const {
        double v = 0.0;
        for (double p : probs) {
            v += p * (1.0 - p);
        }
        return v;
    }
};

}  // namespace distributions
