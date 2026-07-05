#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct GeneralizedGammaDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    double c_;
    double scale_;
    GeneralizedGammaDistribution(double a, double c, double scale) : a_(a), c_(c), scale_(scale) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(c_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(scale_ * std::pow(detail::sample_gamma(rng, a_, 1.0), 1.0 / c_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using GeneralizedGamma = GeneralizedGammaDistribution<double>;

}  // namespace distributions
