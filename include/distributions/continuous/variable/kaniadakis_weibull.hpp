#pragma once

#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct KaniadakisWeibull {
    double kappa_;
    double shape_;
    double scale_;
    KaniadakisWeibull(double kappa, double shape, double scale) : kappa_(kappa), shape_(shape), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_kaniadakis_weibull(rng, kappa_, shape_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
