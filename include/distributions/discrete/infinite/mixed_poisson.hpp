#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MixedPoisson {
    double shape_;
    double scale_;
    MixedPoisson(double shape, double scale) : shape_(shape), scale_(scale) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_mixed_poisson(rng, shape_, scale_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
