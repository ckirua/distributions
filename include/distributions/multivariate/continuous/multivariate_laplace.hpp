#pragma once

#include "distributions/detail/multivariate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MultivariateLaplace {
    double scale_;
    MultivariateLaplace(double scale) : scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_multivariate_laplace_first(rng, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
