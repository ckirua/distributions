#pragma once

#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>

namespace distributions {

struct Exponential {
    double rate;

    explicit Exponential(double rate = 1.0) : rate(rate) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return -std::log1p(-rng.next_double()) / rate;
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 1.0 / rate; }

    [[nodiscard]] double variance() const { return 1.0 / (rate * rate); }
};

}  // namespace distributions
