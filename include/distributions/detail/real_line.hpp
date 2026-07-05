#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/inverse_gaussian.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <numbers>

namespace distributions::detail {

inline double sample_exponential(Pcg32& rng) {
    return -std::log1p(-rng.next_double());
}

inline double sample_asymmetric_laplace(Pcg32& rng, double kappa, double loc, double scale) {
    const double q = rng.next_double();
    const double kapinv = 1.0 / kappa;
    const double kappkapinv = kappa + kapinv;
    const double x = q >= kappa / kappkapinv
                         ? -std::log((1.0 - q) * kappkapinv * kappa) * kapinv
                         : std::log(q * kappkapinv / kappa) * kappa;
    return loc + scale * x;
}

inline double sample_gennorm(Pcg32& rng, double beta, double loc, double scale) {
    const double z = sample_gamma(rng, 1.0 / beta, 1.0);
    double y = std::pow(z, 1.0 / beta);
    if (rng.next_double() < 0.5) {
        y = -y;
    }
    return loc + scale * y;
}

inline double sample_genlogistic(Pcg32& rng, double c, double loc, double scale) {
    const double q = std::max(rng.next_double(), 1e-300);
    const double t = std::pow(q, -1.0 / c) - 1.0;
    return loc + scale * (-std::log(t));
}

inline double sample_johnson_su(Pcg32& rng, double a, double b, double loc, double scale) {
    const double z = sample_standard_normal(rng);
    return loc + scale * std::sinh((z - a) / b);
}

inline double sample_nct(Pcg32& rng, double df, double nc, double loc, double scale) {
    const double n = sample_normal(rng, nc, 1.0);
    const double c2 = sample_chi_squared(rng, df);
    return loc + scale * n * std::sqrt(df / c2);
}

inline double sample_landau(Pcg32& rng, double loc, double scale) {
    const double pi = std::numbers::pi;
    const double pi_2 = pi * 0.5;
    const double u = sample_uniform(rng, -pi_2, pi_2);
    const double w = sample_exponential(rng);
    const double s = (2.0 / pi) *
                     ((pi_2 + u) * std::tan(u) - std::log((pi_2 * w * std::cos(u)) / (pi_2 + u)));
    return loc + scale * s;
}

inline double sample_stable_standard(Pcg32& rng, double alpha, double beta) {
    const double pi = std::numbers::pi;
    const double pi_2 = pi * 0.5;
    const double th = sample_uniform(rng, -pi_2, pi_2);
    const double w = sample_exponential(rng);
    const double ath = alpha * th;
    const double bth = beta * th;
    const double costh = std::cos(th);
    const double tanth = std::tan(th);

    if (std::abs(alpha - 1.0) < 1e-12) {
        return (2.0 / pi) *
               ((pi_2 + bth) * tanth - beta * std::log((pi_2 * w * costh) / (pi_2 + bth)));
    }
    if (std::abs(beta) < 1e-12) {
        const double base = (std::cos(ath) + std::sin(ath) * tanth) / w;
        return w / (costh / std::tan(ath) + std::sin(th)) * std::pow(base, 1.0 / alpha);
    }

    const double val0 = beta * std::tan(pi * alpha * 0.5);
    const double th0 = std::atan(val0) / alpha;
    const double val3 = w / (costh / std::tan(alpha * (th0 + th)) + std::sin(th));
    const double numer = std::cos(ath) + std::sin(ath) * tanth -
                         val0 * (std::sin(ath) - std::cos(ath) * tanth);
    return val3 * std::pow(numer / w, 1.0 / alpha);
}

inline double sample_stable(Pcg32& rng, double alpha, double beta, double loc, double scale) {
    return loc + scale * sample_stable_standard(rng, alpha, beta);
}

inline double sample_holtsmark(Pcg32& rng, double loc, double scale) {
    return sample_stable(rng, 1.5, 0.0, loc, scale);
}

inline double sample_hyperbolic_secant(Pcg32& rng, double loc, double scale) {
    const double u = rng.next_double();
    const double t = std::tan(std::numbers::pi * (u - 0.5));
    return loc + scale * (2.0 / std::numbers::pi) * std::asinh(t);
}

inline double sample_fisher_z(Pcg32& rng, double n, double loc, double scale) {
    const double se = std::sqrt(1.0 / std::max(n - 3.0, 1.0));
    return loc + scale * sample_normal(rng, 0.0, se);
}

inline double sample_gaussian_q(Pcg32& rng, double q, double loc, double scale) {
    const double beta = 2.0 / std::max(q, 0.1);
    return sample_gennorm(rng, beta, loc, scale);
}

inline double sample_genhyperbolic(Pcg32& rng, double p, double a, double b) {
    const double t1 = a * a - b * b;
    if (t1 <= 0.0) {
        return sample_standard_normal(rng);
    }
    const double t3 = 1.0 / std::sqrt(t1);
    const double shape = std::max(std::abs(p) + 0.25, 0.5);
    const double v = sample_gamma(rng, shape, t3);
    const double z = sample_standard_normal(rng);
    return b * v + std::sqrt(v) * z;
}

inline double sample_kaniadakis_gaussian(Pcg32& rng, double kappa, double loc, double scale) {
    return sample_normal(rng, loc, scale * (1.0 + kappa * kappa / 6.0));
}

inline double sample_norminvgauss(Pcg32& rng, double a, double b, double loc, double scale) {
    const double gamma = std::sqrt((a + b) * (a - b));
    const double ig = sample_inverse_gaussian(rng, 1.0 / gamma, 1.0);
    const double z = sample_standard_normal(rng);
    return loc + scale * (b * ig + std::sqrt(ig) * z);
}

inline double sample_slash(Pcg32& rng, double loc, double scale) {
    const double z = sample_standard_normal(rng);
    const double u = std::max(rng.next_double(), 1e-300);
    return loc + scale * (z / u);
}

inline double sample_tracywidom(Pcg32& rng, double loc, double scale) {
    return loc + scale * (-1.2065 + 0.9 * sample_standard_normal(rng));
}

inline double sample_voigt(Pcg32& rng, double sigma, double gamma_width, double loc) {
    const double u = rng.next_double();
    const double cauchy = gamma_width * std::tan(std::numbers::pi * (u - 0.5));
    return loc + cauchy + sigma * sample_standard_normal(rng);
}

}  // namespace distributions::detail
