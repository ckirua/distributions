#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NoncentralChiSquared {
    double df_;
    double ncp_;
    NoncentralChiSquared(double df, double ncp) : df_(df), ncp_(ncp) {
        detail::assert_strictly_positive(df_);
        detail::assert_nonnegative(ncp_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const int j = detail::sample_poisson(rng, ncp_ * 0.5);
        return detail::sample_chi_squared(rng, df_ + 2.0 * static_cast<double>(j));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
