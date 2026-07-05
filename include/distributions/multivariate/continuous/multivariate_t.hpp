#pragma once

#include "distributions/detail/multivariate.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MultivariateT {
    double df_;
    MultivariateT(double df) : df_(df) {
        detail::assert_strictly_positive(df_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_multivariate_t_first(rng, df_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
