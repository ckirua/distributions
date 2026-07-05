#pragma once

#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Bingham {
    double kappa_;
    Bingham(double kappa) : kappa_(kappa) {
        detail::assert_nonnegative(kappa_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_bingham_x(rng, kappa_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
