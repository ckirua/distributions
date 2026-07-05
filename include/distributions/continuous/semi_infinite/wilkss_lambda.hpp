#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct WilkssLambda {
    double loc_;
    double scale_;
    WilkssLambda(double loc, double scale) : loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return loc_ + scale_ * detail::sample_standard_normal(rng);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
