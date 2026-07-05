#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"

#include <cmath>

namespace distributions::detail {

struct Sym2 {
    double m00;
    double m01;
    double m11;
};

inline Sym2 sample_wishart2(Pcg32& rng, double df, double v00, double v01, double v11) {
    const double l11 = std::sqrt(sample_chi_squared(rng, df));
    const double l21 = sample_standard_normal(rng);
    const double l22 = std::sqrt(sample_chi_squared(rng, std::max(df - 1.0, 1.0)));
    const double c00 = std::sqrt(std::max(v00, 1e-12));
    const double c10 = v01 / c00;
    const double c11 = std::sqrt(std::max(v11 - c10 * c10, 1e-12));
    const double cl00 = c00 * l11;
    const double cl10 = c10 * l11 + c11 * l21;
    const double cl11 = c11 * l22;
    return {cl00 * cl00, cl00 * cl10, cl10 * cl10 + cl11 * cl11};
}

inline double sym2_trace(const Sym2& m) {
    return m.m00 + m.m11;
}

inline double sym2_inv_trace(const Sym2& m) {
    const double det = std::max(m.m00 * m.m11 - m.m01 * m.m01, 1e-12);
    return (m.m00 + m.m11) / det;
}

inline double sample_wishart_trace(Pcg32& rng, double df, double v00, double v01, double v11) {
    return sym2_trace(sample_wishart2(rng, df, v00, v01, v11));
}

inline double sample_invwishart_trace(Pcg32& rng, double df, double v00, double v01, double v11) {
    return sym2_inv_trace(sample_wishart2(rng, df, v00, v01, v11));
}

inline double sample_matrix_normal_elem(Pcg32& rng, double row_var, double col_var) {
    return sample_normal(rng, 0.0, std::sqrt(std::max(row_var * col_var, 1e-12)));
}

inline double sample_matrix_t_elem(Pcg32& rng, double df, double row_var, double col_var) {
    const double z = sample_standard_normal(rng);
    const double v = sample_chi_squared(rng, df);
    return z * std::sqrt((row_var * col_var) * df / v);
}

inline double sample_matrix_gamma_trace(Pcg32& rng, double shape, double v00, double v11) {
    return sample_gamma(rng, shape, v00) + sample_gamma(rng, shape - 0.5, v11);
}

inline double sample_inv_matrix_gamma_trace(Pcg32& rng, double shape, double v00, double v11) {
    const double g0 = sample_gamma(rng, shape, v00);
    const double g1 = sample_gamma(rng, shape - 0.5, v11);
    return 1.0 / std::max(g0, 1e-12) + 1.0 / std::max(g1, 1e-12);
}

inline double sample_matrix_beta_trace(Pcg32& rng, double a, double b) {
    const double x = sample_gamma(rng, a, 1.0);
    const double y = sample_gamma(rng, b, 1.0);
    return 2.0 * x / std::max(x + y, 1e-12);
}

inline double sample_matrix_f_trace(Pcg32& rng, double df1, double df2, double v00, double v11) {
    const double t1 = sample_wishart_trace(rng, df1, v00, 0.0, v11);
    const double t2 = sample_wishart_trace(rng, df2, v00, 0.0, v11);
    return t1 / std::max(t2, 1e-12);
}

inline double sample_lkj_rho(Pcg32& rng, double eta) {
    return 2.0 * sample_beta(rng, eta, eta) - 1.0;
}

inline double sample_normal_wishart_mean(Pcg32& rng, double df, double kappa, double v00, double v11) {
    const double tr = sample_wishart_trace(rng, df, v00, 0.0, v11);
    return sample_normal(rng, 0.0, std::sqrt(kappa / std::max(tr, 1e-12)));
}

inline double sample_normal_inverse_mean(Pcg32& rng, double df, double kappa, double v00, double v11) {
    const double tr = sample_invwishart_trace(rng, df, v00, 0.0, v11);
    return sample_normal(rng, 0.0, std::sqrt(tr / std::max(kappa, 1e-12)));
}

inline double sample_complex_wishart_trace(Pcg32& rng, double df, double v00, double v11) {
    return sample_gamma(rng, df, 2.0 * v00) + sample_gamma(rng, df, 2.0 * v11);
}

inline double sample_stiefel_element(Pcg32& rng) {
    const double z1 = sample_standard_normal(rng);
    const double z2 = sample_standard_normal(rng);
    const double n = std::sqrt(z1 * z1 + z2 * z2);
    return z1 / std::max(n, 1e-12);
}

}  // namespace distributions::detail
