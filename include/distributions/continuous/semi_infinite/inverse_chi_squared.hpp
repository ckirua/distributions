#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct InverseChiSquared {
    double df_;
    double scale_;
    InverseChiSquared(double df, double scale) : df_(df), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return scale_ / detail::sample_chi_squared(rng, df_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
