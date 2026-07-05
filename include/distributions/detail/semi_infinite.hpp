#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/detail/student_t.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <numbers>

namespace distributions::detail {

inline double sample_exponential_rate(Pcg32& rng, double rate) {
    return -std::log1p(-rng.next_double()) / rate;
}

inline double sample_invweibull(Pcg32& rng, double shape, double scale) {
    const double u = rng.next_double();
    return scale * std::pow(-std::log(u), -1.0 / shape);
}

inline double sample_weibull_min(Pcg32& rng, double shape, double scale) {
    const double u = rng.next_double();
    return scale * std::pow(-std::log1p(-u), 1.0 / shape);
}

inline double sample_benini(Pcg32& rng, double alpha, double beta, double gamma) {
    const double y = sample_exponential_rate(rng, beta);
    return alpha * (std::exp(std::pow(y, 1.0 / gamma)) - 1.0);
}

inline double sample_benktander(Pcg32& rng, double a, double b) {
    return sample_weibull_min(rng, b, a);
}

inline double sample_davis(Pcg32& rng, double nu) {
    return sample_gamma(rng, nu / 2.0, 2.0);
}

inline double sample_discrete_weibull(Pcg32& rng, double c) {
    const double u = rng.next_double();
    double total = 0.0;
    double cumulative = 0.0;
    for (int k = -50; k <= 50; ++k) {
        total += std::pow(static_cast<double>(std::abs(k)), c - 1.0);
    }
    for (int k = -50; k <= 50; ++k) {
        cumulative += std::pow(static_cast<double>(std::abs(k)), c - 1.0) / total;
        if (u <= cumulative) {
            return static_cast<double>(k);
        }
    }
    return 0.0;
}

inline double sample_hyper_mixture(Pcg32& rng, double lambda1, double lambda2, double w1) {
    if (rng.next_double() < w1) {
        return sample_exponential_rate(rng, lambda1);
    }
    return sample_exponential_rate(rng, lambda2);
}

inline double sample_hypoexponential(Pcg32& rng, double rate1, double rate2) {
    return sample_exponential_rate(rng, rate1) + sample_exponential_rate(rng, rate2);
}

inline double sample_log_cauchy(Pcg32& rng, double loc, double scale) {
    const double u = rng.next_double();
    const double z = loc + scale * std::tan(std::numbers::pi * (u - 0.5));
    return std::exp(z);
}

inline double sample_log_t(Pcg32& rng, double df) {
    return std::exp(sample_student_t(rng, df));
}

inline double sample_noncentral_f(Pcg32& rng, double dfn, double dfd, double nc) {
    const double x = sample_ncx2(rng, dfn, nc);
    const double y = sample_chi_squared(rng, dfd);
    return (x / dfn) / (y / dfd);
}

inline double sample_shifted_exponential(Pcg32& rng, double loc, double rate) {
    return loc + sample_exponential_rate(rng, rate);
}

inline double sample_rel_breitwigner(Pcg32& rng, double rho) {
    const double r = std::max(rho, 1e-6);
    const double c = std::sqrt(2.0 * (1.0 + 1.0 / (r * r)) / (1.0 + std::sqrt(1.0 + 1.0 / (r * r)))) * 2.0 / std::numbers::pi;
    for (;;) {
        const double x = sample_uniform(rng, 0.0, 10.0 * r);
        const double t = ((x - r) * (x + r) / r);
        const double pdf = c / (t * t + 1.0);
        if (rng.next_double() <= pdf / c) {
            return x;
        }
    }
}

inline double sample_hotelling_t_squared(Pcg32& rng, double df1, double df2) {
    return sample_noncentral_f(rng, df1, df2, 0.0);
}

inline double sample_kolmogorov(Pcg32& rng, double scale) {
    const double u = rng.next_double();
    return scale * std::sqrt(-0.5 * std::log(u * (1.0 - u)));
}

inline double sample_hartman_watson(Pcg32& rng, double nu) {
    return 1.0 + sample_chi_squared(rng, std::max(nu, 1.0)) / (2.0 * std::max(nu, 1.0));
}

inline double sample_maxwell_juttner(Pcg32& rng, double a) {
    const double z1 = sample_standard_normal(rng);
    const double z2 = sample_standard_normal(rng);
    const double z3 = sample_standard_normal(rng);
    return a * std::sqrt(z1 * z1 + z2 * z2 + z3 * z3);
}

inline double sample_mittag_leffler(Pcg32& rng, double alpha, double scale) {
    return scale * sample_gamma(rng, std::max(alpha, 0.25), 1.0);
}

inline double sample_phase_type(Pcg32& rng, double rate1, double rate2, double rate3) {
    return sample_exponential_rate(rng, rate1) + sample_exponential_rate(rng, rate2) +
           sample_exponential_rate(rng, rate3);
}

inline double sample_poly_weibull(Pcg32& rng, double shape1, double shape2, double weight) {
    if (rng.next_double() < weight) {
        return sample_weibull_min(rng, shape1, 1.0);
    }
    return sample_weibull_min(rng, shape2, 1.0);
}

inline double sample_wilks_lambda(Pcg32& rng, double a, double b) {
    const double u = sample_chi_squared(rng, 2.0 * a);
    const double v = sample_chi_squared(rng, 2.0 * b);
    return u / (u + v);
}

}  // namespace distributions::detail
