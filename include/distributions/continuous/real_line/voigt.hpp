#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Voigt {
    double sigma_;
    double gamma_width_;
    double loc_;
    Voigt(double sigma, double gamma_width, double loc) : sigma_(sigma), gamma_width_(gamma_width), loc_(loc) {
        detail::assert_strictly_positive(sigma_);
        detail::assert_finite(gamma_width_);
        detail::assert_finite(loc_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_voigt(rng, sigma_, gamma_width_, loc_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
