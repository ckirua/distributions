#pragma once

#include "distributions/detail/circular.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct WrappedLevy {
    double loc_;
    double scale_;
    WrappedLevy(double loc, double scale) : loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double z = detail::sample_standard_normal(rng);
        return detail::wrap_angle(loc_ + scale_ / (z * z));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
