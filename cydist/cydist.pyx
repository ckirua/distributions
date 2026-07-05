# cython: language_level=3
"""Cython batch samplers for all vault distributions (generated).

Pre-allocated NumPy ``out`` is filled in-place; GIL released per call.
Discrete → int32, continuous → float64. RNG: PCG32 (``seed``, default 42).
"""
cimport numpy as cnp
from libc.stddef cimport size_t
from libc.stdint cimport uint64_t

cnp.import_array()

cdef extern from "cydist_shim.h":
    void cydist_benford_sample_batch(uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_bernoulli_sample_batch(double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_beta_binomial_sample_batch(int trials, double alpha, double beta, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_binomial_sample_batch(int trials, double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_categorical_sample_batch(const double* probs, int k, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_hypergeometric_sample_batch(int M, int n_success, int N_draws, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_negative_hypergeometric_sample_batch(int M, int n_, int N, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_poisson_binomial_sample_batch(const double* probs, int k, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_rademacher_sample_batch(uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_soliton_sample_batch(double loc, double scale, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_discrete_uniform_sample_batch(int low, int high, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_zipf_sample_batch(int N, double s, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_zipf_mandelbrot_sample_batch(int N, double q, double s, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_beta_negative_binomial_sample_batch(double alpha, double beta, double r, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_borel_sample_batch(double mu, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_conwaymaxwellpoisson_sample_batch(double lambda_, double nu, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_discrete_phase_type_sample_batch(double p1, double p2, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_delaporte_sample_batch(double lambda_, double r, double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_extended_negative_binomial_sample_batch(double r, double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_floryschulz_sample_batch(double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_gausskuzmin_sample_batch(uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_geometric_sample_batch(double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_logarithmic_sample_batch(double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_mixed_poisson_sample_batch(double shape, double scale, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_negative_binomial_sample_batch(int r, double p, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_panjer_sample_batch(double lambda_, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_parabolic_fractal_sample_batch(double b, double c, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_poisson_sample_batch(double mu, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_skellam_sample_batch(double mu1, double mu2, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_yulesimon_sample_batch(double rho, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_zeta_sample_batch(int N, double s, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_arcsine_sample_batch(uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_argus_sample_batch(double chi, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_baldingnichols_sample_batch(double f, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_bates_sample_batch(int n_, double lo, double hi, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_beta_sample_batch(double alpha, double beta, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_beta_sample_batch(double alpha, double beta, double lambda_, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_beta_rectangular_sample_batch(double alpha, double beta, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_continuous_bernoulli_sample_batch(double lambda_, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_continuous_binomial_sample_batch(int n_, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_irwinhall_sample_batch(int n_, double lo, double hi, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kumaraswamy_sample_batch(double a, double b, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_logit_normal_sample_batch(double mu, double sigma, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_noncentral_beta_sample_batch(double alpha, double beta, double lam, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_pert_sample_batch(double lo, double mode, double hi, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_raised_cosine_sample_batch(double lo, double hi, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_reciprocal_sample_batch(double a, double b, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_triangular_sample_batch(double lo, double mode, double hi, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_u_quadratic_sample_batch(double a, double b, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_uniform_sample_batch(double lo, double hi, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wigner_semicircle_sample_batch(double lo, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_benini_sample_batch(double alpha, double beta, double gamma, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_benktander_1st_kind_sample_batch(double a, double b, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_benktander_2nd_kind_sample_batch(double a, double b, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_beta_prime_sample_batch(double alpha, double beta, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_burr_sample_batch(double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_chi_sample_batch(double df, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_chi_squared_sample_batch(double df, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_noncentral_chi_squared_sample_batch(double df, double ncp, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_inverse_chi_squared_sample_batch(double df, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_scaled_inverse_chi_squared_sample_batch(double df, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_dagum_sample_batch(double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_davis_sample_batch(double nu, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_erlang_sample_batch(double a, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_hyper_sample_batch(double lambda1, double lambda2, double w1, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_exponential_sample_batch(double rate, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_hyperexponential_sample_batch(double lambda1, double lambda2, double w1, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_hypoexponential_sample_batch(double rate1, double rate2, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_exponential_logarithmic_sample_batch(double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_f_sample_batch(double dfn, double dfd, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_noncentral_f_sample_batch(double dfn, double dfd, double nc, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_folded_normal_sample_batch(double c, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_frechet_sample_batch(double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_gamma_sample_batch(double shape, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_gamma_sample_batch(double a, double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_inverse_gamma_sample_batch(double shape, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_gamma_gompertz_sample_batch(double shape, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_gompertz_sample_batch(double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_shifted_sample_batch(double loc, double rate, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_half_logistic_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_half_normal_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_hotellings_t_squared_sample_batch(double df1, double df2, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_hartmanwatson_sample_batch(double nu, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_inverse_gaussian_sample_batch(double mu, double lambda_, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_sample_batch(double a, double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kolmogorov_sample_batch(double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_levy_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_log_cauchy_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_log_laplace_sample_batch(double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_log_logistic_sample_batch(double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_log_normal_sample_batch(double mu, double sigma, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_log_t_sample_batch(double df, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_lomax_sample_batch(double c, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_matrix_exponential_sample_batch(double rate1, double rate2, double rate3, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_maxwellboltzmann_sample_batch(double a, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_maxwelljuttner_sample_batch(double a, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_mittag_leffler_sample_batch(double alpha, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_nakagami_sample_batch(double nu, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_pareto_sample_batch(double alpha, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_phase_type_sample_batch(double rate1, double rate2, double rate3, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_poly_weibull_sample_batch(double shape1, double shape2, double weight, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_rayleigh_sample_batch(double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_relativistic_breitwigner_sample_batch(double rho, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_rice_sample_batch(double b, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_truncated_normal_sample_batch(double a, double b, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_type_2_gumbel_sample_batch(double shape, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_weibull_sample_batch(double shape, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_discrete_weibull_sample_batch(double c, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wilkss_lambda_sample_batch(double a, double b, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_cauchy_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_exponential_power_sample_batch(double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_fishers_z_sample_batch(double n_, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kaniadakis_gaussian_sample_batch(double kappa, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_gaussian_q_sample_batch(double q, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_hyperbolic_sample_batch(double p, double a, double b, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_logistic_logistic_beta_sample_batch(double c, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_normal_sample_batch(double beta, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_geometric_stable_sample_batch(double alpha, double beta, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_gumbel_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_holtsmark_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_hyperbolic_secant_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_johnsons_su_sample_batch(double a, double b, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_landau_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_laplace_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_asymmetric_sample_batch(double kappa, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_logistic_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_noncentral_t_sample_batch(double df, double nc, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_normal_sample_batch(double mu, double sigma, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_normal_inverse_gaussian_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_skew_normal_sample_batch(double loc, double scale, double alpha, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_slash_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_stable_sample_batch(double alpha, double beta, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_students_t_sample_batch(double df, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_tracywidom_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_variance_gamma_sample_batch(double a, double b, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_voigt_sample_batch(double sigma, double gamma_width, double loc, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_chi_squared_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_extreme_value_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_pareto_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_marchenkopastur_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kaniadakis_exponential_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kaniadakis_gamma_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kaniadakis_weibull_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kaniadakis_logistic_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kaniadakis_erlang_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_q_exponential_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_q_gaussian_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_q_weibull_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_shifted_log_logistic_sample_batch(double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_tukey_lambda_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_rectified_gaussian_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_ewens_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_multinomial_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_dirichlet_multinomial_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_negative_multinomial_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_dirichlet_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_generalized_dirichlet_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_multivariate_laplace_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_multivariate_normal_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_multivariate_stable_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_multivariate_t_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_normal_gamma_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_normal_inverse_gamma_distribution_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_lkj_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_matrix_beta_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_matrix_f_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_matrix_normal_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_matrix_t_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_matrix_gamma_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_inverse_matrix_gamma_distribution_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wishart_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_normal_wishart_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_inverse_wishart_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_normal_inverse_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_complex_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_uniform_distribution_on_a_stiefel_manifold_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_circular_uniform_sample_batch(double mu, double kappa, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_univariate_von_mises_sample_batch(double mu, double kappa, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wrapped_normal_sample_batch(double mu, double sigma, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wrapped_cauchy_sample_batch(double c, double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wrapped_exponential_sample_batch(double loc, double rate, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wrapped_asymmetric_laplace_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_wrapped_levy_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_kent_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_bivariate_von_mises_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_von_misesfisher_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_bingham_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_dirac_delta_function_sample_batch(double x0, uint64_t seed, double* out, size_t n_samples) nogil
    void cydist_cantor_sample_batch(double loc, double scale, uint64_t seed, double* out, size_t n_samples) nogil

# --- discrete / finite-support ---

def benford_sample_batch(cnp.int32_t[:] out, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_benford_sample_batch(seed, ptr, n_samples)

def bernoulli_sample_batch(cnp.int32_t[:] out, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_bernoulli_sample_batch(p, seed, ptr, n_samples)

def beta_binomial_sample_batch(cnp.int32_t[:] out, int trials, double alpha, double beta, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_beta_binomial_sample_batch(trials, alpha, beta, seed, ptr, n_samples)

def binomial_sample_batch(cnp.int32_t[:] out, int trials, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_binomial_sample_batch(trials, p, seed, ptr, n_samples)

def categorical_sample_batch(cnp.int32_t[:] out, cnp.float64_t[:] probs, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    cdef const double* probs_ptr = <const double*>&probs[0]
    cdef int k = <int>probs.shape[0]
    with nogil:
        cydist_categorical_sample_batch(probs_ptr, k, seed, ptr, n_samples)

def hypergeometric_sample_batch(cnp.int32_t[:] out, int M, int n_success, int N_draws, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_hypergeometric_sample_batch(M, n_success, N_draws, seed, ptr, n_samples)

def negative_hypergeometric_sample_batch(cnp.int32_t[:] out, int M, int n_, int N, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_negative_hypergeometric_sample_batch(M, n_, N, seed, ptr, n_samples)

def poisson_binomial_sample_batch(cnp.int32_t[:] out, cnp.float64_t[:] probs, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    cdef const double* probs_ptr = <const double*>&probs[0]
    cdef int k = <int>probs.shape[0]
    with nogil:
        cydist_poisson_binomial_sample_batch(probs_ptr, k, seed, ptr, n_samples)

def rademacher_sample_batch(cnp.int32_t[:] out, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_rademacher_sample_batch(seed, ptr, n_samples)

def soliton_sample_batch(cnp.int32_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_soliton_sample_batch(loc, scale, seed, ptr, n_samples)

def discrete_uniform_sample_batch(cnp.int32_t[:] out, int low, int high, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_discrete_uniform_sample_batch(low, high, seed, ptr, n_samples)

def zipf_sample_batch(cnp.int32_t[:] out, int N, double s, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_zipf_sample_batch(N, s, seed, ptr, n_samples)

def zipf_mandelbrot_sample_batch(cnp.int32_t[:] out, int N, double q, double s, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_zipf_mandelbrot_sample_batch(N, q, s, seed, ptr, n_samples)


# --- discrete / infinite-support ---

def beta_negative_binomial_sample_batch(cnp.int32_t[:] out, double alpha, double beta, double r, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_beta_negative_binomial_sample_batch(alpha, beta, r, seed, ptr, n_samples)

def borel_sample_batch(cnp.int32_t[:] out, double mu, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_borel_sample_batch(mu, seed, ptr, n_samples)

def conwaymaxwellpoisson_sample_batch(cnp.int32_t[:] out, double lambda_, double nu, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_conwaymaxwellpoisson_sample_batch(lambda_, nu, seed, ptr, n_samples)

def discrete_phase_type_sample_batch(cnp.int32_t[:] out, double p1, double p2, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_discrete_phase_type_sample_batch(p1, p2, seed, ptr, n_samples)

def delaporte_sample_batch(cnp.int32_t[:] out, double lambda_, double r, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_delaporte_sample_batch(lambda_, r, p, seed, ptr, n_samples)

def extended_negative_binomial_sample_batch(cnp.int32_t[:] out, double r, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_extended_negative_binomial_sample_batch(r, p, seed, ptr, n_samples)

def floryschulz_sample_batch(cnp.int32_t[:] out, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_floryschulz_sample_batch(p, seed, ptr, n_samples)

def gausskuzmin_sample_batch(cnp.int32_t[:] out, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_gausskuzmin_sample_batch(seed, ptr, n_samples)

def geometric_sample_batch(cnp.int32_t[:] out, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_geometric_sample_batch(p, seed, ptr, n_samples)

def logarithmic_sample_batch(cnp.int32_t[:] out, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_logarithmic_sample_batch(p, seed, ptr, n_samples)

def mixed_poisson_sample_batch(cnp.int32_t[:] out, double shape, double scale, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_mixed_poisson_sample_batch(shape, scale, seed, ptr, n_samples)

def negative_binomial_sample_batch(cnp.int32_t[:] out, int r, double p, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_negative_binomial_sample_batch(r, p, seed, ptr, n_samples)

def panjer_sample_batch(cnp.int32_t[:] out, double lambda_, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_panjer_sample_batch(lambda_, seed, ptr, n_samples)

def parabolic_fractal_sample_batch(cnp.int32_t[:] out, double b, double c, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_parabolic_fractal_sample_batch(b, c, seed, ptr, n_samples)

def poisson_sample_batch(cnp.int32_t[:] out, double mu, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_poisson_sample_batch(mu, seed, ptr, n_samples)

def skellam_sample_batch(cnp.int32_t[:] out, double mu1, double mu2, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_skellam_sample_batch(mu1, mu2, seed, ptr, n_samples)

def yulesimon_sample_batch(cnp.int32_t[:] out, double rho, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_yulesimon_sample_batch(rho, seed, ptr, n_samples)

def zeta_sample_batch(cnp.int32_t[:] out, int N, double s, uint64_t seed=42):
    cdef int* ptr = <int*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_zeta_sample_batch(N, s, seed, ptr, n_samples)


# --- continuous / bounded-interval ---

def arcsine_sample_batch(cnp.float64_t[:] out, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_arcsine_sample_batch(seed, ptr, n_samples)

def argus_sample_batch(cnp.float64_t[:] out, double chi, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_argus_sample_batch(chi, seed, ptr, n_samples)

def baldingnichols_sample_batch(cnp.float64_t[:] out, double f, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_baldingnichols_sample_batch(f, seed, ptr, n_samples)

def bates_sample_batch(cnp.float64_t[:] out, int n_, double lo, double hi, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_bates_sample_batch(n_, lo, hi, seed, ptr, n_samples)

def beta_sample_batch(cnp.float64_t[:] out, double alpha, double beta, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_beta_sample_batch(alpha, beta, seed, ptr, n_samples)

def generalized_beta_sample_batch(cnp.float64_t[:] out, double alpha, double beta, double lambda_, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_beta_sample_batch(alpha, beta, lambda_, seed, ptr, n_samples)

def beta_rectangular_sample_batch(cnp.float64_t[:] out, double alpha, double beta, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_beta_rectangular_sample_batch(alpha, beta, seed, ptr, n_samples)

def continuous_bernoulli_sample_batch(cnp.float64_t[:] out, double lambda_, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_continuous_bernoulli_sample_batch(lambda_, seed, ptr, n_samples)

def continuous_binomial_sample_batch(cnp.float64_t[:] out, int n_, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_continuous_binomial_sample_batch(n_, seed, ptr, n_samples)

def irwinhall_sample_batch(cnp.float64_t[:] out, int n_, double lo, double hi, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_irwinhall_sample_batch(n_, lo, hi, seed, ptr, n_samples)

def kumaraswamy_sample_batch(cnp.float64_t[:] out, double a, double b, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kumaraswamy_sample_batch(a, b, seed, ptr, n_samples)

def logit_normal_sample_batch(cnp.float64_t[:] out, double mu, double sigma, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_logit_normal_sample_batch(mu, sigma, seed, ptr, n_samples)

def noncentral_beta_sample_batch(cnp.float64_t[:] out, double alpha, double beta, double lam, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_noncentral_beta_sample_batch(alpha, beta, lam, seed, ptr, n_samples)

def pert_sample_batch(cnp.float64_t[:] out, double lo, double mode, double hi, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_pert_sample_batch(lo, mode, hi, seed, ptr, n_samples)

def raised_cosine_sample_batch(cnp.float64_t[:] out, double lo, double hi, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_raised_cosine_sample_batch(lo, hi, seed, ptr, n_samples)

def reciprocal_sample_batch(cnp.float64_t[:] out, double a, double b, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_reciprocal_sample_batch(a, b, seed, ptr, n_samples)

def triangular_sample_batch(cnp.float64_t[:] out, double lo, double mode, double hi, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_triangular_sample_batch(lo, mode, hi, seed, ptr, n_samples)

def u_quadratic_sample_batch(cnp.float64_t[:] out, double a, double b, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_u_quadratic_sample_batch(a, b, seed, ptr, n_samples)

def uniform_sample_batch(cnp.float64_t[:] out, double lo, double hi, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_uniform_sample_batch(lo, hi, seed, ptr, n_samples)

def wigner_semicircle_sample_batch(cnp.float64_t[:] out, double lo, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wigner_semicircle_sample_batch(lo, scale, seed, ptr, n_samples)


# --- continuous / semi-infinite-interval ---

def benini_sample_batch(cnp.float64_t[:] out, double alpha, double beta, double gamma, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_benini_sample_batch(alpha, beta, gamma, seed, ptr, n_samples)

def benktander_1st_kind_sample_batch(cnp.float64_t[:] out, double a, double b, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_benktander_1st_kind_sample_batch(a, b, seed, ptr, n_samples)

def benktander_2nd_kind_sample_batch(cnp.float64_t[:] out, double a, double b, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_benktander_2nd_kind_sample_batch(a, b, seed, ptr, n_samples)

def beta_prime_sample_batch(cnp.float64_t[:] out, double alpha, double beta, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_beta_prime_sample_batch(alpha, beta, seed, ptr, n_samples)

def burr_sample_batch(cnp.float64_t[:] out, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_burr_sample_batch(c, scale, seed, ptr, n_samples)

def chi_sample_batch(cnp.float64_t[:] out, double df, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_chi_sample_batch(df, seed, ptr, n_samples)

def chi_squared_sample_batch(cnp.float64_t[:] out, double df, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_chi_squared_sample_batch(df, seed, ptr, n_samples)

def noncentral_chi_squared_sample_batch(cnp.float64_t[:] out, double df, double ncp, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_noncentral_chi_squared_sample_batch(df, ncp, seed, ptr, n_samples)

def inverse_chi_squared_sample_batch(cnp.float64_t[:] out, double df, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_inverse_chi_squared_sample_batch(df, scale, seed, ptr, n_samples)

def scaled_inverse_chi_squared_sample_batch(cnp.float64_t[:] out, double df, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_scaled_inverse_chi_squared_sample_batch(df, scale, seed, ptr, n_samples)

def dagum_sample_batch(cnp.float64_t[:] out, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_dagum_sample_batch(c, scale, seed, ptr, n_samples)

def davis_sample_batch(cnp.float64_t[:] out, double nu, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_davis_sample_batch(nu, seed, ptr, n_samples)

def erlang_sample_batch(cnp.float64_t[:] out, double a, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_erlang_sample_batch(a, scale, seed, ptr, n_samples)

def hyper_sample_batch(cnp.float64_t[:] out, double lambda1, double lambda2, double w1, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_hyper_sample_batch(lambda1, lambda2, w1, seed, ptr, n_samples)

def exponential_sample_batch(cnp.float64_t[:] out, double rate, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_exponential_sample_batch(rate, seed, ptr, n_samples)

def hyperexponential_sample_batch(cnp.float64_t[:] out, double lambda1, double lambda2, double w1, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_hyperexponential_sample_batch(lambda1, lambda2, w1, seed, ptr, n_samples)

def hypoexponential_sample_batch(cnp.float64_t[:] out, double rate1, double rate2, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_hypoexponential_sample_batch(rate1, rate2, seed, ptr, n_samples)

def exponential_logarithmic_sample_batch(cnp.float64_t[:] out, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_exponential_logarithmic_sample_batch(scale, seed, ptr, n_samples)

def f_sample_batch(cnp.float64_t[:] out, double dfn, double dfd, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_f_sample_batch(dfn, dfd, seed, ptr, n_samples)

def noncentral_f_sample_batch(cnp.float64_t[:] out, double dfn, double dfd, double nc, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_noncentral_f_sample_batch(dfn, dfd, nc, seed, ptr, n_samples)

def folded_normal_sample_batch(cnp.float64_t[:] out, double c, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_folded_normal_sample_batch(c, loc, scale, seed, ptr, n_samples)

def frechet_sample_batch(cnp.float64_t[:] out, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_frechet_sample_batch(c, scale, seed, ptr, n_samples)

def gamma_sample_batch(cnp.float64_t[:] out, double shape, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_gamma_sample_batch(shape, scale, seed, ptr, n_samples)

def generalized_gamma_sample_batch(cnp.float64_t[:] out, double a, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_gamma_sample_batch(a, c, scale, seed, ptr, n_samples)

def inverse_gamma_sample_batch(cnp.float64_t[:] out, double shape, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_inverse_gamma_sample_batch(shape, scale, seed, ptr, n_samples)

def gamma_gompertz_sample_batch(cnp.float64_t[:] out, double shape, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_gamma_gompertz_sample_batch(shape, scale, seed, ptr, n_samples)

def gompertz_sample_batch(cnp.float64_t[:] out, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_gompertz_sample_batch(c, scale, seed, ptr, n_samples)

def shifted_sample_batch(cnp.float64_t[:] out, double loc, double rate, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_shifted_sample_batch(loc, rate, seed, ptr, n_samples)

def half_logistic_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_half_logistic_sample_batch(loc, scale, seed, ptr, n_samples)

def half_normal_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_half_normal_sample_batch(loc, scale, seed, ptr, n_samples)

def hotellings_t_squared_sample_batch(cnp.float64_t[:] out, double df1, double df2, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_hotellings_t_squared_sample_batch(df1, df2, seed, ptr, n_samples)

def hartmanwatson_sample_batch(cnp.float64_t[:] out, double nu, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_hartmanwatson_sample_batch(nu, seed, ptr, n_samples)

def inverse_gaussian_sample_batch(cnp.float64_t[:] out, double mu, double lambda_, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_inverse_gaussian_sample_batch(mu, lambda_, seed, ptr, n_samples)

def generalized_sample_batch(cnp.float64_t[:] out, double a, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_sample_batch(a, c, scale, seed, ptr, n_samples)

def kolmogorov_sample_batch(cnp.float64_t[:] out, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kolmogorov_sample_batch(scale, seed, ptr, n_samples)

def levy_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_levy_sample_batch(loc, scale, seed, ptr, n_samples)

def log_cauchy_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_log_cauchy_sample_batch(loc, scale, seed, ptr, n_samples)

def log_laplace_sample_batch(cnp.float64_t[:] out, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_log_laplace_sample_batch(c, scale, seed, ptr, n_samples)

def log_logistic_sample_batch(cnp.float64_t[:] out, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_log_logistic_sample_batch(c, scale, seed, ptr, n_samples)

def log_normal_sample_batch(cnp.float64_t[:] out, double mu, double sigma, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_log_normal_sample_batch(mu, sigma, seed, ptr, n_samples)

def log_t_sample_batch(cnp.float64_t[:] out, double df, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_log_t_sample_batch(df, seed, ptr, n_samples)

def lomax_sample_batch(cnp.float64_t[:] out, double c, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_lomax_sample_batch(c, scale, seed, ptr, n_samples)

def matrix_exponential_sample_batch(cnp.float64_t[:] out, double rate1, double rate2, double rate3, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_matrix_exponential_sample_batch(rate1, rate2, rate3, seed, ptr, n_samples)

def maxwellboltzmann_sample_batch(cnp.float64_t[:] out, double a, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_maxwellboltzmann_sample_batch(a, seed, ptr, n_samples)

def maxwelljuttner_sample_batch(cnp.float64_t[:] out, double a, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_maxwelljuttner_sample_batch(a, seed, ptr, n_samples)

def mittag_leffler_sample_batch(cnp.float64_t[:] out, double alpha, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_mittag_leffler_sample_batch(alpha, scale, seed, ptr, n_samples)

def nakagami_sample_batch(cnp.float64_t[:] out, double nu, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_nakagami_sample_batch(nu, scale, seed, ptr, n_samples)

def pareto_sample_batch(cnp.float64_t[:] out, double alpha, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_pareto_sample_batch(alpha, scale, seed, ptr, n_samples)

def phase_type_sample_batch(cnp.float64_t[:] out, double rate1, double rate2, double rate3, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_phase_type_sample_batch(rate1, rate2, rate3, seed, ptr, n_samples)

def poly_weibull_sample_batch(cnp.float64_t[:] out, double shape1, double shape2, double weight, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_poly_weibull_sample_batch(shape1, shape2, weight, seed, ptr, n_samples)

def rayleigh_sample_batch(cnp.float64_t[:] out, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_rayleigh_sample_batch(scale, seed, ptr, n_samples)

def relativistic_breitwigner_sample_batch(cnp.float64_t[:] out, double rho, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_relativistic_breitwigner_sample_batch(rho, seed, ptr, n_samples)

def rice_sample_batch(cnp.float64_t[:] out, double b, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_rice_sample_batch(b, scale, seed, ptr, n_samples)

def truncated_normal_sample_batch(cnp.float64_t[:] out, double a, double b, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_truncated_normal_sample_batch(a, b, loc, scale, seed, ptr, n_samples)

def type_2_gumbel_sample_batch(cnp.float64_t[:] out, double shape, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_type_2_gumbel_sample_batch(shape, scale, seed, ptr, n_samples)

def weibull_sample_batch(cnp.float64_t[:] out, double shape, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_weibull_sample_batch(shape, scale, seed, ptr, n_samples)

def discrete_weibull_sample_batch(cnp.float64_t[:] out, double c, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_discrete_weibull_sample_batch(c, seed, ptr, n_samples)

def wilkss_lambda_sample_batch(cnp.float64_t[:] out, double a, double b, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wilkss_lambda_sample_batch(a, b, seed, ptr, n_samples)


# --- continuous / whole-real-line ---

def cauchy_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_cauchy_sample_batch(loc, scale, seed, ptr, n_samples)

def exponential_power_sample_batch(cnp.float64_t[:] out, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_exponential_power_sample_batch(scale, seed, ptr, n_samples)

def fishers_z_sample_batch(cnp.float64_t[:] out, double n_, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_fishers_z_sample_batch(n_, loc, scale, seed, ptr, n_samples)

def kaniadakis_gaussian_sample_batch(cnp.float64_t[:] out, double kappa, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kaniadakis_gaussian_sample_batch(kappa, loc, scale, seed, ptr, n_samples)

def gaussian_q_sample_batch(cnp.float64_t[:] out, double q, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_gaussian_q_sample_batch(q, loc, scale, seed, ptr, n_samples)

def generalized_hyperbolic_sample_batch(cnp.float64_t[:] out, double p, double a, double b, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_hyperbolic_sample_batch(p, a, b, seed, ptr, n_samples)

def generalized_logistic_logistic_beta_sample_batch(cnp.float64_t[:] out, double c, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_logistic_logistic_beta_sample_batch(c, loc, scale, seed, ptr, n_samples)

def generalized_normal_sample_batch(cnp.float64_t[:] out, double beta, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_normal_sample_batch(beta, loc, scale, seed, ptr, n_samples)

def geometric_stable_sample_batch(cnp.float64_t[:] out, double alpha, double beta, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_geometric_stable_sample_batch(alpha, beta, loc, scale, seed, ptr, n_samples)

def gumbel_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_gumbel_sample_batch(loc, scale, seed, ptr, n_samples)

def holtsmark_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_holtsmark_sample_batch(loc, scale, seed, ptr, n_samples)

def hyperbolic_secant_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_hyperbolic_secant_sample_batch(loc, scale, seed, ptr, n_samples)

def johnsons_su_sample_batch(cnp.float64_t[:] out, double a, double b, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_johnsons_su_sample_batch(a, b, loc, scale, seed, ptr, n_samples)

def landau_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_landau_sample_batch(loc, scale, seed, ptr, n_samples)

def laplace_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_laplace_sample_batch(loc, scale, seed, ptr, n_samples)

def asymmetric_sample_batch(cnp.float64_t[:] out, double kappa, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_asymmetric_sample_batch(kappa, loc, scale, seed, ptr, n_samples)

def logistic_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_logistic_sample_batch(loc, scale, seed, ptr, n_samples)

def noncentral_t_sample_batch(cnp.float64_t[:] out, double df, double nc, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_noncentral_t_sample_batch(df, nc, loc, scale, seed, ptr, n_samples)

def normal_sample_batch(cnp.float64_t[:] out, double mu, double sigma, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_normal_sample_batch(mu, sigma, seed, ptr, n_samples)

def normal_inverse_gaussian_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_normal_inverse_gaussian_sample_batch(loc, scale, seed, ptr, n_samples)

def skew_normal_sample_batch(cnp.float64_t[:] out, double loc, double scale, double alpha, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_skew_normal_sample_batch(loc, scale, alpha, seed, ptr, n_samples)

def slash_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_slash_sample_batch(loc, scale, seed, ptr, n_samples)

def stable_sample_batch(cnp.float64_t[:] out, double alpha, double beta, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_stable_sample_batch(alpha, beta, loc, scale, seed, ptr, n_samples)

def students_t_sample_batch(cnp.float64_t[:] out, double df, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_students_t_sample_batch(df, seed, ptr, n_samples)

def tracywidom_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_tracywidom_sample_batch(loc, scale, seed, ptr, n_samples)

def variance_gamma_sample_batch(cnp.float64_t[:] out, double a, double b, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_variance_gamma_sample_batch(a, b, loc, scale, seed, ptr, n_samples)

def voigt_sample_batch(cnp.float64_t[:] out, double sigma, double gamma_width, double loc, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_voigt_sample_batch(sigma, gamma_width, loc, seed, ptr, n_samples)


# --- continuous / variable-support ---

def generalized_chi_squared_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_chi_squared_sample_batch(loc, scale, seed, ptr, n_samples)

def generalized_extreme_value_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_extreme_value_sample_batch(loc, scale, seed, ptr, n_samples)

def generalized_pareto_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_pareto_sample_batch(loc, scale, seed, ptr, n_samples)

def marchenkopastur_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_marchenkopastur_sample_batch(loc, scale, seed, ptr, n_samples)

def kaniadakis_exponential_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kaniadakis_exponential_sample_batch(loc, scale, seed, ptr, n_samples)

def kaniadakis_gamma_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kaniadakis_gamma_sample_batch(loc, scale, seed, ptr, n_samples)

def kaniadakis_weibull_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kaniadakis_weibull_sample_batch(loc, scale, seed, ptr, n_samples)

def kaniadakis_logistic_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kaniadakis_logistic_sample_batch(loc, scale, seed, ptr, n_samples)

def kaniadakis_erlang_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kaniadakis_erlang_sample_batch(loc, scale, seed, ptr, n_samples)

def q_exponential_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_q_exponential_sample_batch(loc, scale, seed, ptr, n_samples)

def q_gaussian_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_q_gaussian_sample_batch(loc, scale, seed, ptr, n_samples)

def q_weibull_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_q_weibull_sample_batch(loc, scale, seed, ptr, n_samples)

def shifted_log_logistic_sample_batch(cnp.float64_t[:] out, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_shifted_log_logistic_sample_batch(scale, seed, ptr, n_samples)

def tukey_lambda_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_tukey_lambda_sample_batch(loc, scale, seed, ptr, n_samples)


# --- mixed / continuous-discrete ---

def rectified_gaussian_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_rectified_gaussian_sample_batch(loc, scale, seed, ptr, n_samples)


# --- multivariate / discrete ---

def ewens_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_ewens_sample_batch(loc, scale, seed, ptr, n_samples)

def multinomial_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_multinomial_sample_batch(loc, scale, seed, ptr, n_samples)

def dirichlet_multinomial_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_dirichlet_multinomial_sample_batch(loc, scale, seed, ptr, n_samples)

def negative_multinomial_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_negative_multinomial_sample_batch(loc, scale, seed, ptr, n_samples)


# --- multivariate / continuous ---

def dirichlet_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_dirichlet_sample_batch(loc, scale, seed, ptr, n_samples)

def generalized_dirichlet_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_generalized_dirichlet_sample_batch(loc, scale, seed, ptr, n_samples)

def multivariate_laplace_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_multivariate_laplace_sample_batch(loc, scale, seed, ptr, n_samples)

def multivariate_normal_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_multivariate_normal_sample_batch(loc, scale, seed, ptr, n_samples)

def multivariate_stable_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_multivariate_stable_sample_batch(loc, scale, seed, ptr, n_samples)

def multivariate_t_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_multivariate_t_sample_batch(loc, scale, seed, ptr, n_samples)

def normal_gamma_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_normal_gamma_sample_batch(loc, scale, seed, ptr, n_samples)

def normal_inverse_gamma_distribution_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_normal_inverse_gamma_distribution_sample_batch(loc, scale, seed, ptr, n_samples)


# --- multivariate / matrix-valued ---

def lkj_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_lkj_sample_batch(loc, scale, seed, ptr, n_samples)

def matrix_beta_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_matrix_beta_sample_batch(loc, scale, seed, ptr, n_samples)

def matrix_f_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_matrix_f_sample_batch(loc, scale, seed, ptr, n_samples)

def matrix_normal_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_matrix_normal_sample_batch(loc, scale, seed, ptr, n_samples)

def matrix_t_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_matrix_t_sample_batch(loc, scale, seed, ptr, n_samples)

def matrix_gamma_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_matrix_gamma_sample_batch(loc, scale, seed, ptr, n_samples)

def inverse_matrix_gamma_distribution_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_inverse_matrix_gamma_distribution_sample_batch(loc, scale, seed, ptr, n_samples)

def wishart_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wishart_sample_batch(loc, scale, seed, ptr, n_samples)

def normal_wishart_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_normal_wishart_sample_batch(loc, scale, seed, ptr, n_samples)

def inverse_wishart_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_inverse_wishart_sample_batch(loc, scale, seed, ptr, n_samples)

def normal_inverse_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_normal_inverse_sample_batch(loc, scale, seed, ptr, n_samples)

def complex_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_complex_sample_batch(loc, scale, seed, ptr, n_samples)

def uniform_distribution_on_a_stiefel_manifold_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_uniform_distribution_on_a_stiefel_manifold_sample_batch(loc, scale, seed, ptr, n_samples)


# --- directional / univariate-circular ---

def circular_uniform_sample_batch(cnp.float64_t[:] out, double mu, double kappa, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_circular_uniform_sample_batch(mu, kappa, seed, ptr, n_samples)

def univariate_von_mises_sample_batch(cnp.float64_t[:] out, double mu, double kappa, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_univariate_von_mises_sample_batch(mu, kappa, seed, ptr, n_samples)

def wrapped_normal_sample_batch(cnp.float64_t[:] out, double mu, double sigma, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wrapped_normal_sample_batch(mu, sigma, seed, ptr, n_samples)

def wrapped_cauchy_sample_batch(cnp.float64_t[:] out, double c, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wrapped_cauchy_sample_batch(c, loc, scale, seed, ptr, n_samples)

def wrapped_exponential_sample_batch(cnp.float64_t[:] out, double loc, double rate, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wrapped_exponential_sample_batch(loc, rate, seed, ptr, n_samples)

def wrapped_asymmetric_laplace_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wrapped_asymmetric_laplace_sample_batch(loc, scale, seed, ptr, n_samples)

def wrapped_levy_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_wrapped_levy_sample_batch(loc, scale, seed, ptr, n_samples)


# --- directional / bivariate-spherical ---

def kent_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_kent_sample_batch(loc, scale, seed, ptr, n_samples)


# --- directional / bivariate-toroidal ---

def bivariate_von_mises_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_bivariate_von_mises_sample_batch(loc, scale, seed, ptr, n_samples)


# --- directional / multivariate ---

def von_misesfisher_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_von_misesfisher_sample_batch(loc, scale, seed, ptr, n_samples)

def bingham_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_bingham_sample_batch(loc, scale, seed, ptr, n_samples)


# --- degenerate-and-singular / degenerate ---

def dirac_delta_function_sample_batch(cnp.float64_t[:] out, double x0, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_dirac_delta_function_sample_batch(x0, seed, ptr, n_samples)


# --- degenerate-and-singular / singular ---

def cantor_sample_batch(cnp.float64_t[:] out, double loc, double scale, uint64_t seed=42):
    cdef double* ptr = <double*>&out[0]
    cdef size_t n_samples = <size_t>out.shape[0]
    with nogil:
        cydist_cantor_sample_batch(loc, scale, seed, ptr, n_samples)
