#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/real_line.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct VoigtDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double sigma_;
    double gamma_width_;
    double loc_;
    VoigtDistribution(double sigma, double gamma_width, double loc) : sigma_(sigma), gamma_width_(gamma_width), loc_(loc) {
        detail::assert_strictly_positive(sigma_);
        detail::assert_finite(gamma_width_);
        detail::assert_finite(loc_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_voigt(rng, sigma_, gamma_width_, loc_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Voigt = VoigtDistribution<double>;

}  // namespace distributions
