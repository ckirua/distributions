#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct KaniadakisGaussian {
    double kappa_;
    double loc_;
    double scale_;
    KaniadakisGaussian(double kappa, double loc, double scale) : kappa_(kappa), loc_(loc), scale_(scale) {
        detail::assert_nonnegative(kappa_);
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_kaniadakis_gaussian(rng, kappa_, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
