#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct HalfNormal {
    double loc_;
    double scale_;
    HalfNormal(double loc, double scale) : loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return loc_ + scale_ * std::abs(detail::sample_standard_normal(rng));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
