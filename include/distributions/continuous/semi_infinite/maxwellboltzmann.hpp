#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct MaxwellBoltzmann {
    double a_;
    MaxwellBoltzmann(double a) : a_(a) {
        detail::assert_strictly_positive(a_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double x = detail::sample_standard_normal(rng);
        const double y = detail::sample_standard_normal(rng);
        const double z = detail::sample_standard_normal(rng);
        return a_ * std::sqrt(x * x + y * y + z * z);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
