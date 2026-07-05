#pragma once

#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct DiracDelta {
    double x0_;
    DiracDelta(double x0) : x0_(x0) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return x0_;
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
