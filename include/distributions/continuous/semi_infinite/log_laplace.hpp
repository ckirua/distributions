#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct LogLaplace {
    double c_;
    double scale_;
    LogLaplace(double c, double scale) : c_(c), scale_(scale) {
        detail::assert_strictly_positive(c_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = rng.next_double();
        if (u < 0.5) { return scale_ * std::exp(std::log(2.0 * u) / c_); }
        return scale_ * std::exp(-std::log(2.0 * (1.0 - u)) / c_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
