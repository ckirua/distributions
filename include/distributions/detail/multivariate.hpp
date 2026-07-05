#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/real_line.hpp"
#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

inline double sample_inverse_gamma(Pcg32& rng, double shape, double scale) {
    return scale / sample_gamma(rng, shape, 1.0);
}

inline double sample_dirichlet_first(Pcg32& rng, double a0, double a1, double a2) {
    const double g0 = sample_gamma(rng, a0, 1.0);
    const double g1 = sample_gamma(rng, a1, 1.0);
    const double g2 = sample_gamma(rng, a2, 1.0);
    return g0 / (g0 + g1 + g2);
}

inline double sample_generalized_dirichlet_first(Pcg32& rng, double a1, double b1, double a2, double b2) {
    const double y1 = sample_beta(rng, a1, b1);
    (void)sample_beta(rng, a2, b2);
    return y1;
}

inline double sample_multivariate_normal_first(Pcg32& rng, double /*rho*/) {
    return sample_standard_normal(rng);
}

inline double sample_multivariate_t_first(Pcg32& rng, double df) {
    const double z = sample_standard_normal(rng);
    const double v = sample_chi_squared(rng, df);
    return z * std::sqrt(df / v);
}

inline double sample_multivariate_laplace_first(Pcg32& rng, double scale) {
    const double e = sample_exponential(rng);
    return scale * sample_standard_normal(rng) * std::sqrt(e);
}

inline double sample_multivariate_stable_first(Pcg32& rng, double alpha, double beta) {
    return sample_stable_standard(rng, alpha, beta);
}

inline double sample_normal_gamma_mean(Pcg32& rng, double shape, double rate) {
    const double tau = sample_gamma(rng, shape, rate);
    return sample_normal(rng, 0.0, 1.0 / std::sqrt(std::max(tau, 1e-12)));
}

inline double sample_normal_invgamma_mean(Pcg32& rng, double shape, double scale) {
    const double v = sample_inverse_gamma(rng, shape, scale);
    return sample_normal(rng, 0.0, std::sqrt(std::max(v, 1e-12)));
}

}  // namespace distributions::detail
