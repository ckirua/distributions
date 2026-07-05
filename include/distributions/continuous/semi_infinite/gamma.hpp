#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Gamma {
    double shape_;
    double scale_;
    Gamma(double shape, double scale) : shape_(shape), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_gamma(rng, shape_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
