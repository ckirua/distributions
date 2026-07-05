#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

inline double sample_ncx2(Pcg32& rng, double df, double ncp) {
    const int n = sample_poisson(rng, ncp / 2.0);
    return sample_chi_squared(rng, df + 2.0 * static_cast<double>(n));
}

inline double sample_argus(Pcg32& rng, double chi) {
    const double chi2 = chi * chi;
    if (chi > 1.8) {
        for (;;) {
            const double g = sample_gamma(rng, 1.5, 1.0);
            if (g <= chi2 / 2.0) {
                return std::sqrt(1.0 - 2.0 * g / chi2);
            }
        }
    }
    if (chi <= 0.5) {
        const double d = -chi2 / 2.0;
        for (;;) {
            const double u = rng.next_double();
            const double v = rng.next_double();
            const double z = std::pow(v, 2.0 / 3.0);
            if (std::log(u) <= d * z) {
                return std::sqrt(1.0 - z);
            }
        }
    }
    const double echi = std::exp(-chi2 / 2.0);
    for (;;) {
        const double u = rng.next_double();
        const double v = rng.next_double();
        const double z = 2.0 * std::log(echi * (1.0 - v) + v) / chi2;
        if (u * u + z <= 0.0) {
            return std::sqrt(1.0 + z);
        }
    }
}

inline double sample_balding_nichols(Pcg32& rng, double f) {
    const double u = std::max(f, 1e-6);
    const double alpha = (1.0 - u) / (2.0 * u);
    return sample_beta(rng, alpha, alpha);
}

inline double sample_generalized_beta(Pcg32& rng, double alpha, double beta, double lambda) {
    if (std::abs(lambda) < 1e-12) {
        return sample_beta(rng, alpha, beta);
    }
    for (;;) {
        const double x = sample_beta(rng, alpha, beta);
        const double t = 1.0 - lambda * x;
        if (t <= 0.0) {
            continue;
        }
        if (rng.next_double() <= std::pow(t, -(alpha + beta))) {
            return x;
        }
    }
}

inline double sample_continuous_bernoulli(Pcg32& rng, double lambda) {
    const double lam = std::min(std::max(lambda, 1e-6), 1.0 - 1e-6);
    const int n = 512;
    double total = 0.0;
    double weights[512];
    for (int i = 0; i < n; ++i) {
        const double x = (static_cast<double>(i) + 0.5) / static_cast<double>(n);
        weights[i] = std::exp(x * std::log(lam) + (1.0 - x) * std::log1p(-lam));
        total += weights[i];
    }
    const double u = rng.next_double() * total;
    double cumulative = 0.0;
    for (int i = 0; i < n; ++i) {
        cumulative += weights[i];
        if (u <= cumulative) {
            return (static_cast<double>(i) + 0.5) / static_cast<double>(n);
        }
    }
    return 0.5;
}

inline double sample_continuous_binomial(Pcg32& rng, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += sample_uniform(rng, 0.0, 1.0);
    }
    return sum / static_cast<double>(n);
}

inline double sample_noncentral_beta(Pcg32& rng, double alpha, double beta, double lam) {
    if (lam <= 1e-12) {
        return sample_beta(rng, alpha, beta);
    }
    const double u = sample_ncx2(rng, 2.0 * alpha, lam);
    const double v = sample_chi_squared(rng, 2.0 * beta);
    return u / (u + v);
}

inline double sample_pert(Pcg32& rng, double lo, double mode, double hi) {
    const double span = hi - lo;
    const double a = 1.0 + 4.0 * (mode - lo) / span;
    const double b = 1.0 + 4.0 * (hi - mode) / span;
    return lo + span * sample_beta(rng, a, b);
}

}  // namespace distributions::detail
