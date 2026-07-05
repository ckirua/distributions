#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <vector>

namespace distributions::detail {

inline int sample_geometric(Pcg32& rng, double p) {
    return static_cast<int>(std::floor(std::log1p(-rng.next_double()) / std::log1p(-p))) + 1;
}

inline int sample_negative_binomial(Pcg32& rng, double r, double p) {
    int k = 0;
    double s = 0.0;
    while (s < r) {
        s += sample_gamma(rng, 1.0, 1.0);
        ++k;
    }
    int successes = 0;
    for (int i = 0; i < k - 1; ++i) {
        if (rng.next_double() < p) {
            ++successes;
        }
    }
    return successes;
}

inline int sample_logarithmic(Pcg32& rng, double p) {
    const double log1mp = std::log1p(-p);
    const double u = rng.next_double();
    double cumulative = 0.0;
    for (int k = 1; k < 10'000; ++k) {
        cumulative -= std::pow(p, static_cast<double>(k)) /
                      (static_cast<double>(k) * log1mp);
        if (u <= cumulative) {
            return k;
        }
    }
    return 1;
}

inline int sample_benford(Pcg32& rng) {
    const double u = rng.next_double();
    static constexpr double cdf[10] = {
        0.0,
        0.3010299956639812,
        0.47712125471966244,
        0.6020599913279624,
        0.6989700043360189,
        0.7781512503836436,
        0.8450980400142568,
        0.9030899869919436,
        0.9542425094393249,
        1.0,
    };
    for (int d = 1; d <= 9; ++d) {
        if (u <= cdf[d]) {
            return d;
        }
    }
    return 9;
}

inline int sample_rademacher(Pcg32& rng) {
    return rng.next_double() < 0.5 ? -1 : 1;
}

inline int sample_yule_simon(Pcg32& rng, double rho) {
    const double u = rng.next_double();
    const double v = rng.next_double();
    const double x = std::exp(-std::log(u) / rho);
    return static_cast<int>(std::floor(1.0 + std::log1p(-v) / std::log1p(-std::exp(-rho / x)))) + 1;
}

inline int sample_negative_binomial_gamma_poisson(Pcg32& rng, double r, double p) {
    const double lambda = sample_gamma(rng, r, 1.0);
    return sample_poisson(rng, lambda * (1.0 - p) / p);
}

inline int sample_flory_schulz(Pcg32& rng, double p) {
    if (rng.next_double() < p) {
        return 1;
    }
    return 2 + sample_negative_binomial_gamma_poisson(rng, 2.0, p);
}

inline int sample_gauss_kuzmin(Pcg32& rng) {
    const double u = rng.next_double();
    double cumulative = 0.0;
    const double log2 = std::log(2.0);
    for (int k = 1; k < 10'000; ++k) {
        const double pk = std::log((static_cast<double>((k + 1) * (k + 1))) /
                                   (static_cast<double>(k) * static_cast<double>(k + 2))) /
                          log2;
        cumulative += pk;
        if (u <= cumulative) {
            return k;
        }
    }
    return 1;
}

inline int sample_borel(Pcg32& rng, double mu) {
    const double u = rng.next_double();
    double cumulative = 0.0;
    for (int k = 1; k < 500; ++k) {
        const double log_pk =
            static_cast<double>(k - 2) * std::log(static_cast<double>(k)) -
            std::lgamma(static_cast<double>(k + 1)) +
            static_cast<double>(k - 1) * std::log(mu) - static_cast<double>(k) * mu;
        const double pk = std::exp(log_pk);
        cumulative += pk;
        if (u <= cumulative) {
            return k;
        }
    }
    return 1;
}

inline int sample_beta_negative_binomial(Pcg32& rng, double alpha, double beta, double r) {
    const double p = sample_beta(rng, alpha, beta);
    return sample_negative_binomial_gamma_poisson(rng, r, p);
}

inline int sample_mixed_poisson(Pcg32& rng, double shape, double scale) {
    const double lambda = sample_gamma(rng, shape, scale);
    return sample_poisson(rng, lambda);
}

inline int sample_delaporte(Pcg32& rng, double lambda, double r, double p) {
    return sample_poisson(rng, lambda) + sample_negative_binomial_gamma_poisson(rng, r, p);
}

inline int sample_comp_poisson(Pcg32& rng, double lambda, double nu) {
    if (std::abs(nu - 1.0) < 1e-12) {
        return sample_poisson(rng, lambda);
    }
    const int k_max = 200;
    double total = 0.0;
    double log_lambda = std::log(lambda > 0.0 ? lambda : 1e-300);
    double weights[201];
    for (int k = 0; k <= k_max; ++k) {
        const double log_w = static_cast<double>(k) * log_lambda -
                             nu * std::lgamma(static_cast<double>(k + 1));
        weights[k] = std::exp(log_w);
        total += weights[k];
    }
    const double u = rng.next_double() * total;
    double cumulative = 0.0;
    for (int k = 0; k <= k_max; ++k) {
        cumulative += weights[k];
        if (u <= cumulative) {
            return k;
        }
    }
    return k_max;
}

inline int sample_discrete_phase_type(Pcg32& rng, double p1, double p2) {
    return sample_geometric(rng, p1) + sample_geometric(rng, p2);
}

inline int sample_parabolic_fractal(Pcg32& rng, double b, double c) {
    const double u = rng.next_double();
    double cumulative = 0.0;
    for (int n = 1; n < 5'000; ++n) {
        const double log_n = std::log(static_cast<double>(n));
        const double pk = std::exp(-b * log_n - c * log_n * log_n);
        cumulative += pk;
        if (u <= cumulative) {
            return n;
        }
    }
    return 1;
}

}  // namespace distributions::detail
