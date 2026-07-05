#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Kolmogorov {
    double scale_;
    Kolmogorov(double scale) : scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_kolmogorov(rng, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
