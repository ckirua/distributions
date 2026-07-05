#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct LogLogistic {
    double c_;
    double scale_;
    LogLogistic(double c, double scale) : c_(c), scale_(scale) {
        detail::assert_strictly_positive(c_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = rng.next_double();
        return scale_ * std::pow(u / (1.0 - u), 1.0 / c_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
