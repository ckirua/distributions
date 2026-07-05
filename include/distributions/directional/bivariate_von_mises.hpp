#pragma once

#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct BivariateVonMises {
    double kappa1_;
    double kappa2_;
    BivariateVonMises(double kappa1, double kappa2) : kappa1_(kappa1), kappa2_(kappa2) {
        detail::assert_nonnegative(kappa1_);
        detail::assert_nonnegative(kappa2_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_bivariate_von_mises_first(rng, kappa1_, kappa2_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
