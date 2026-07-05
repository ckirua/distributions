#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/real_line.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

inline double boxcox(double x, double lam) {
    const double xx = std::max(x, 1e-300);
    if (std::abs(lam) < 1e-12) {
        return std::log(xx);
    }
    return (std::pow(xx, lam) - 1.0) / lam;
}

inline double boxcox1p(double x, double lam) {
    const double xx = std::max(1.0 + x, 1e-300);
    if (std::abs(lam) < 1e-12) {
        return std::log(xx);
    }
    return (std::pow(xx, lam) - 1.0) / lam;
}

inline double sample_logistic(Pcg32& rng, double loc, double scale) {
    const double u = rng.next_double();
    return loc + scale * std::log(u / (1.0 - u));
}

inline double sample_genextreme(Pcg32& rng, double c, double loc, double scale) {
    const double q = std::max(rng.next_double(), 1e-300);
    const double x = -std::log(-std::log(q));
    const double y = std::abs(c) < 1e-12 ? x : -std::expm1(-c * x) / c;
    return loc + scale * y;
}

inline double sample_genpareto(Pcg32& rng, double c, double loc, double scale) {
    const double q = rng.next_double();
    return loc + scale * (-boxcox1p(-q, -c));
}

inline double sample_tukey_lambda(Pcg32& rng, double lam, double loc, double scale) {
    const double q = rng.next_double();
    return loc + scale * (boxcox(q, lam) - boxcox1p(-q, lam));
}

inline double sample_generalized_chi_squared(Pcg32& rng, double df1, double w1, double df2, double w2) {
    return w1 * sample_chi_squared(rng, df1) + w2 * sample_chi_squared(rng, df2);
}

inline double sample_marchenko_pastur(Pcg32& rng, double gamma_ratio, double sigma) {
    const double sr = std::sqrt(std::max(gamma_ratio, 1e-6));
    const double s2 = sigma * sigma;
    const double a = s2 * (1.0 - sr) * (1.0 - sr);
    const double b = s2 * (1.0 + sr) * (1.0 + sr);
    const double t = sample_beta(rng, 2.5, 2.5);
    return a + (b - a) * t;
}

inline double kaniadakis_scale_correction(double kappa) {
    return 1.0 + kappa * kappa / 6.0;
}

inline double sample_kaniadakis_exponential(Pcg32& rng, double kappa, double loc, double scale) {
    return loc + scale * sample_exponential(rng) / kaniadakis_scale_correction(kappa);
}

inline double sample_kaniadakis_gamma(Pcg32& rng, double kappa, double shape, double scale) {
    return sample_gamma(rng, shape, scale / kaniadakis_scale_correction(kappa));
}

inline double sample_kaniadakis_weibull(Pcg32& rng, double kappa, double shape, double scale) {
    return sample_weibull_min(rng, shape, scale / kaniadakis_scale_correction(kappa));
}

inline double sample_kaniadakis_logistic(Pcg32& rng, double kappa, double loc, double scale) {
    return sample_logistic(rng, loc, scale / kaniadakis_scale_correction(kappa));
}

inline double sample_kaniadakis_erlang(Pcg32& rng, double kappa, double shape, double scale) {
    return sample_gamma(rng, shape, scale / kaniadakis_scale_correction(kappa));
}

inline double sample_q_exponential(Pcg32& rng, double q, double loc, double scale) {
    const double u = rng.next_double();
    if (std::abs(q - 1.0) < 1e-8) {
        return loc + scale * sample_exponential(rng);
    }
    return loc + scale * (1.0 - std::pow(1.0 - u, 1.0 - q)) / (1.0 - q);
}

inline double sample_q_gaussian(Pcg32& rng, double q, double loc, double scale) {
    return sample_gaussian_q(rng, q, loc, scale);
}

inline double sample_q_weibull(Pcg32& rng, double q, double shape, double scale) {
    const double corr = std::max(1.0 + (1.0 - q) * 0.25, 0.25);
    return sample_weibull_min(rng, shape, scale / corr);
}

}  // namespace distributions::detail
