#pragma once

#include "distributions/detail/special.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct RectifiedGaussian {
    double loc_;
    double scale_;
    RectifiedGaussian(double loc, double scale) : loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_rectified_gaussian(rng, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
