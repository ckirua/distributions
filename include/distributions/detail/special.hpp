#pragma once

#include "distributions/detail/circular.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/inverse_gaussian.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <numbers>

namespace distributions::detail {

inline double sample_von_mises_angle(Pcg32& rng, double mu, double kappa) {
    if (kappa <= 1e-12) {
        return sample_uniform(rng, -std::numbers::pi, std::numbers::pi);
    }
    const double z = sample_standard_normal(rng);
    return wrap_angle(mu + z / std::sqrt(kappa));
}

inline double sample_rectified_gaussian(Pcg32& rng, double loc, double scale) {
    return std::max(0.0, sample_normal(rng, loc, scale));
}

inline double sample_cantor(Pcg32& rng, double loc, double scale) {
    double x = 0.0;
    double pw = 1.0 / 3.0;
    for (int k = 0; k < 24; ++k) {
        if (rng.next_double() < 0.5) {
            x += 2.0 * pw;
        }
        pw /= 3.0;
    }
    return loc + scale * x;
}

inline double sample_von_mises_fisher_x(Pcg32& rng, double kappa) {
    const double u = rng.next_double();
    return 1.0 + std::log(u + (1.0 - u) * std::exp(-2.0 * kappa)) / kappa;
}

inline double sample_kent_x(Pcg32& rng, double kappa, double beta) {
    const double x = sample_von_mises_fisher_x(rng, kappa);
    const double y = sample_standard_normal(rng) * beta * 0.1;
    const double z = sample_standard_normal(rng) * beta * 0.1;
    const double norm = std::sqrt(x * x + y * y + z * z);
    return x / std::max(norm, 1e-12);
}

inline double sample_bingham_x(Pcg32& rng, double kappa) {
    const double u = rng.next_double();
    const double v = sample_standard_normal(rng) * std::exp(kappa * 0.25);
    const double w = std::sqrt(std::max(1.0 - u * u, 0.0));
    const double x = u / std::sqrt(u * u + w * w + v * v);
    return x;
}

inline double sample_bivariate_von_mises_first(Pcg32& rng, double kappa1, double kappa2) {
    (void)kappa2;
    return sample_von_mises_angle(rng, 0.0, kappa1);
}

inline int sample_soliton(Pcg32& rng, int n_max) {
    const double u = rng.next_double();
    double cumulative = 0.0;
    for (int k = 0; k <= n_max; ++k) {
        const double pk = 1.0 / static_cast<double>(n_max + 1);
        cumulative += pk;
        if (u <= cumulative) {
            return k;
        }
    }
    return n_max;
}

}  // namespace distributions::detail
