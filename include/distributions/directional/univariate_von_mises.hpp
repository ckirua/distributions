#pragma once

#include "distributions/detail/normal.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <numbers>
#include <cmath>
#include <cstddef>

namespace distributions {

struct UnivariateVonMises {
    double mu_;
    double kappa_;
    UnivariateVonMises(double mu, double kappa) : mu_(mu), kappa_(kappa) {
        detail::assert_nonnegative(mu_);
        detail::assert_nonnegative(kappa_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        if (kappa_ <= 1e-12) { return detail::sample_uniform(rng, -std::numbers::pi, std::numbers::pi); }
        const double z = detail::sample_standard_normal(rng);
        const double theta = mu_ + z / std::sqrt(kappa_);
        const double two_pi = 2.0 * std::numbers::pi;
        return std::fmod(theta + std::numbers::pi, two_pi) - std::numbers::pi;
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
