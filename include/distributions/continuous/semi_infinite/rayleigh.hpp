#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Rayleigh {
    double scale_;
    Rayleigh(double scale) : scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return scale_ * std::sqrt(-2.0 * std::log1p(-rng.next_double()));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
