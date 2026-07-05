#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <numbers>
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct CircularUniformDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double mu_;
    double kappa_;
    CircularUniformDistribution(double mu, double kappa) : mu_(mu), kappa_(kappa) {
        detail::assert_nonnegative(mu_);
        detail::assert_nonnegative(kappa_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        if (kappa_ <= 1e-12) { return static_cast<Sample>(detail::sample_uniform(rng, -std::numbers::pi, std::numbers::pi)); }
                const double z = detail::sample_standard_normal(rng);
                const double theta = mu_ + z / std::sqrt(kappa_);
                const double two_pi = 2.0 * std::numbers::pi;
                return static_cast<Sample>(std::fmod(theta + std::numbers::pi, two_pi) - std::numbers::pi);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using CircularUniform = CircularUniformDistribution<double>;

}  // namespace distributions
