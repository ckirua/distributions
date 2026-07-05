#pragma once

#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct KaniadakisGamma {
    double kappa_;
    double shape_;
    double scale_;
    KaniadakisGamma(double kappa, double shape, double scale) : kappa_(kappa), shape_(shape), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_kaniadakis_gamma(rng, kappa_, shape_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
