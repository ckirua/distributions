#pragma once

#include "distributions/detail/special.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct VonMisesfisher {
    double kappa_;
    VonMisesfisher(double kappa) : kappa_(kappa) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_von_mises_fisher_x(rng, kappa_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
