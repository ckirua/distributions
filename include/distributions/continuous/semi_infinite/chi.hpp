#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Chi {
    double df_;
    Chi(double df) : df_(df) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_chi(rng, df_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
