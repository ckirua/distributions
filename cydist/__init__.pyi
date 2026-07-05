"""Fast batch samplers for probability distributions (C++/Cython).

Pre-allocate ``out`` and pass it to ``*_sample_batch``; samples are written in-place.
Discrete distributions use ``int32`` arrays, continuous use ``float64``.
The GIL is released during each call. RNG: PCG32 via ``seed`` (default 42).
"""

import numpy as np

def benford_sample_batch(out: np.ndarray, seed: int = 42) -> None:
    """Sample Benford into ``out`` (int32). Benford distribution: A discrete random variable taking finitely many values."""

def bernoulli_sample_batch(out: np.ndarray, p: float, seed: int = 42) -> None:
    """Sample Bernoulli into ``out`` (int32). Models a single yes/no trial with fixed success probability."""

def beta_binomial_sample_batch(out: np.ndarray, trials: int, alpha: float, beta: float, seed: int = 42) -> None:
    """Sample Beta-binomial into ``out`` (int32). Binomial-like counts when the success probability itself is random (Beta-distributed)."""

def binomial_sample_batch(out: np.ndarray, trials: int, p: float, seed: int = 42) -> None:
    """Sample Binomial into ``out`` (int32). Counts successes in a fixed number of independent identical trials."""

def categorical_sample_batch(out: np.ndarray, probs: np.ndarray, seed: int = 42) -> None:
    """Sample Categorical into ``out`` (int32). One draw from finitely many labeled outcomes, generalizing Bernoulli to K categories."""

def hypergeometric_sample_batch(out: np.ndarray, M: int, n_success: int, N_draws: int, seed: int = 42) -> None:
    """Sample Hypergeometric into ``out`` (int32). Hypergeometric distribution: A discrete random variable taking finitely many values."""

def negative_hypergeometric_sample_batch(out: np.ndarray, M: int, n_: int, N: int, seed: int = 42) -> None:
    """Sample Negative hypergeometric into ``out`` (int32). Negative hypergeometric distribution: A discrete random variable taking finitely many values."""

def poisson_binomial_sample_batch(out: np.ndarray, probs: np.ndarray, seed: int = 42) -> None:
    """Sample Poisson binomial into ``out`` (int32). Sum of independent Bernoulli trials with non-identical success probabilities."""

def rademacher_sample_batch(out: np.ndarray, seed: int = 42) -> None:
    """Sample Rademacher into ``out`` (int32). Rademacher distribution: A discrete random variable taking finitely many values."""

def soliton_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Soliton into ``out`` (int32). Soliton distribution: A discrete random variable taking finitely many values. Tier: heuristic."""

def discrete_uniform_sample_batch(out: np.ndarray, low: int, high: int, seed: int = 42) -> None:
    """Sample Discrete uniform into ``out`` (int32). Each of finitely many integers is equally likely."""

def zipf_sample_batch(out: np.ndarray, N: int, s: float, seed: int = 42) -> None:
    """Sample Zipf into ``out`` (int32). Power-law over ranks: common events dominate, rare events trail off slowly."""

def zipf_mandelbrot_sample_batch(out: np.ndarray, N: int, q: float, s: float, seed: int = 42) -> None:
    """Sample Zipf–Mandelbrot into ``out`` (int32). Rank-frequency law with an offset, flexible tail for ranked discrete data."""

def beta_negative_binomial_sample_batch(out: np.ndarray, alpha: float, beta: float, r: float, seed: int = 42) -> None:
    """Sample Beta negative binomial into ``out`` (int32). Beta negative binomial distribution: A discrete random variable on the non-negative integers or s..."""

def borel_sample_batch(out: np.ndarray, mu: float, seed: int = 42) -> None:
    """Sample Borel into ``out`` (int32). Borel distribution: A discrete random variable on the non-negative integers or similar count set."""

def conwaymaxwellpoisson_sample_batch(out: np.ndarray, lambda_: float, nu: float, seed: int = 42) -> None:
    """Sample Conway–Maxwell–Poisson into ``out`` (int32). Conway–Maxwell–Poisson distribution: A discrete random variable on the non-negative integers or s..."""

def discrete_phase_type_sample_batch(out: np.ndarray, p1: float, p2: float, seed: int = 42) -> None:
    """Sample Discrete phase-type into ``out`` (int32). Discrete phase-type distribution: A discrete random variable on the non-negative integers or simi..."""

def delaporte_sample_batch(out: np.ndarray, lambda_: float, r: float, p: float, seed: int = 42) -> None:
    """Sample Delaporte into ``out`` (int32). Delaporte distribution: A discrete random variable on the non-negative integers or similar count..."""

def extended_negative_binomial_sample_batch(out: np.ndarray, r: float, p: float, seed: int = 42) -> None:
    """Sample Extended negative binomial into ``out`` (int32). Extended negative binomial distribution: A discrete random variable on the non-negative integers..."""

def floryschulz_sample_batch(out: np.ndarray, p: float, seed: int = 42) -> None:
    """Sample Flory–Schulz into ``out`` (int32). Flory–Schulz distribution: A discrete random variable on the non-negative integers or similar cou..."""

def gausskuzmin_sample_batch(out: np.ndarray, seed: int = 42) -> None:
    """Sample Gauss–Kuzmin into ``out`` (int32). Gauss–Kuzmin distribution: A discrete random variable on the non-negative integers or similar cou..."""

def geometric_sample_batch(out: np.ndarray, p: float, seed: int = 42) -> None:
    """Sample Geometric into ``out`` (int32). Number of trials until the first success in repeated independent Bernoulli trials."""

def logarithmic_sample_batch(out: np.ndarray, p: float, seed: int = 42) -> None:
    """Sample Logarithmic into ``out`` (int32). Logarithmic distribution: A discrete random variable on the non-negative integers or similar coun..."""

def mixed_poisson_sample_batch(out: np.ndarray, shape: float, scale: float, seed: int = 42) -> None:
    """Sample Mixed Poisson into ``out`` (int32). Mixed Poisson distribution: A discrete random variable on the non-negative integers or similar co..."""

def negative_binomial_sample_batch(out: np.ndarray, r: int, p: float, seed: int = 42) -> None:
    """Sample Negative binomial into ``out`` (int32). Count of failures before a target number of successes in repeated Bernoulli trials."""

def panjer_sample_batch(out: np.ndarray, lambda_: float, seed: int = 42) -> None:
    """Sample Panjer into ``out`` (int32). Panjer (a,b,0) class: A discrete random variable on the non-negative integers or similar count set."""

def parabolic_fractal_sample_batch(out: np.ndarray, b: float, c: float, seed: int = 42) -> None:
    """Sample Parabolic fractal into ``out`` (int32). Parabolic fractal distribution: A discrete random variable on the non-negative integers or simila..."""

def poisson_sample_batch(out: np.ndarray, mu: float, seed: int = 42) -> None:
    """Sample Poisson into ``out`` (int32). Models event counts in a fixed interval when events arrive independently at a steady average rate."""

def skellam_sample_batch(out: np.ndarray, mu1: float, mu2: float, seed: int = 42) -> None:
    """Sample Skellam into ``out`` (int32). Difference of two independent Poisson counts."""

def yulesimon_sample_batch(out: np.ndarray, rho: float, seed: int = 42) -> None:
    """Sample Yule–Simon into ``out`` (int32). Yule–Simon distribution: A discrete random variable on the non-negative integers or similar count..."""

def zeta_sample_batch(out: np.ndarray, N: int, s: float, seed: int = 42) -> None:
    """Sample Zeta into ``out`` (int32). Zeta distribution: A discrete random variable on the non-negative integers or similar count set."""

def arcsine_sample_batch(out: np.ndarray, seed: int = 42) -> None:
    """Sample Arcsine into ``out`` (float64). Arcsine distribution: A continuous random variable confined to a bounded interval."""

def argus_sample_batch(out: np.ndarray, chi: float, seed: int = 42) -> None:
    """Sample ARGUS into ``out`` (float64). ARGUS distribution: A continuous random variable confined to a bounded interval."""

def baldingnichols_sample_batch(out: np.ndarray, f: float, seed: int = 42) -> None:
    """Sample Balding–Nichols into ``out`` (float64). Balding–Nichols distribution: A continuous random variable confined to a bounded interval."""

def bates_sample_batch(out: np.ndarray, n_: int, lo: float, hi: float, seed: int = 42) -> None:
    """Sample Bates into ``out`` (float64). Bates distribution: A continuous random variable confined to a bounded interval."""

def beta_sample_batch(out: np.ndarray, alpha: float, beta: float, seed: int = 42) -> None:
    """Sample Beta into ``out`` (float64). Flexible distribution on the unit interval for proportions, probabilities, and rates bounded betw..."""

def generalized_beta_sample_batch(out: np.ndarray, alpha: float, beta: float, lambda_: float, seed: int = 42) -> None:
    """Sample Generalized beta into ``out`` (float64). Generalized beta distribution: A continuous random variable confined to a bounded interval."""

def beta_rectangular_sample_batch(out: np.ndarray, alpha: float, beta: float, seed: int = 42) -> None:
    """Sample Beta rectangular into ``out`` (float64). Beta rectangular distribution: A continuous random variable confined to a bounded interval."""

def continuous_bernoulli_sample_batch(out: np.ndarray, lambda_: float, seed: int = 42) -> None:
    """Sample Continuous Bernoulli into ``out`` (float64). Continuous Bernoulli distribution: A continuous random variable confined to a bounded interval."""

def continuous_binomial_sample_batch(out: np.ndarray, n_: int, seed: int = 42) -> None:
    """Sample Continuous binomial into ``out`` (float64). Continuous binomial distribution: A continuous random variable confined to a bounded interval."""

def irwinhall_sample_batch(out: np.ndarray, n_: int, lo: float, hi: float, seed: int = 42) -> None:
    """Sample Irwin–Hall into ``out`` (float64). Irwin–Hall distribution: A continuous random variable confined to a bounded interval."""

def kumaraswamy_sample_batch(out: np.ndarray, a: float, b: float, seed: int = 42) -> None:
    """Sample Kumaraswamy into ``out`` (float64). Kumaraswamy distribution: A continuous random variable confined to a bounded interval."""

def logit_normal_sample_batch(out: np.ndarray, mu: float, sigma: float, seed: int = 42) -> None:
    """Sample Logit-normal into ``out`` (float64). Logit-normal distribution: A continuous random variable confined to a bounded interval."""

def noncentral_beta_sample_batch(out: np.ndarray, alpha: float, beta: float, lam: float, seed: int = 42) -> None:
    """Sample Noncentral beta into ``out`` (float64). Noncentral beta distribution: A continuous random variable confined to a bounded interval."""

def pert_sample_batch(out: np.ndarray, lo: float, mode: float, hi: float, seed: int = 42) -> None:
    """Sample PERT into ``out`` (float64). PERT distribution: A continuous random variable confined to a bounded interval."""

def raised_cosine_sample_batch(out: np.ndarray, lo: float, hi: float, seed: int = 42) -> None:
    """Sample Raised cosine into ``out`` (float64). Raised cosine distribution: A continuous random variable confined to a bounded interval."""

def reciprocal_sample_batch(out: np.ndarray, a: float, b: float, seed: int = 42) -> None:
    """Sample Reciprocal into ``out`` (float64). Reciprocal distribution: A continuous random variable confined to a bounded interval."""

def triangular_sample_batch(out: np.ndarray, lo: float, mode: float, hi: float, seed: int = 42) -> None:
    """Sample Triangular into ``out`` (float64). Triangular distribution: A continuous random variable confined to a bounded interval."""

def u_quadratic_sample_batch(out: np.ndarray, a: float, b: float, seed: int = 42) -> None:
    """Sample U-quadratic into ``out`` (float64). U-quadratic distribution: A continuous random variable confined to a bounded interval."""

def uniform_sample_batch(out: np.ndarray, lo: float, hi: float, seed: int = 42) -> None:
    """Sample Uniform into ``out`` (float64). Every value in an interval is equally likely — maximum ignorance on a bounded range."""

def wigner_semicircle_sample_batch(out: np.ndarray, lo: float, scale: float, seed: int = 42) -> None:
    """Sample Wigner semicircle into ``out`` (float64). Wigner semicircle distribution: A continuous random variable confined to a bounded interval."""

def benini_sample_batch(out: np.ndarray, alpha: float, beta: float, gamma: float, seed: int = 42) -> None:
    """Sample Benini into ``out`` (float64). Benini distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def benktander_1st_kind_sample_batch(out: np.ndarray, a: float, b: float, seed: int = 42) -> None:
    """Sample Benktander 1st kind into ``out`` (float64). Benktander 1st kind distribution: A continuous random variable with support on a half-line such a..."""

def benktander_2nd_kind_sample_batch(out: np.ndarray, a: float, b: float, seed: int = 42) -> None:
    """Sample Benktander 2nd kind into ``out`` (float64). Benktander 2nd kind distribution: A continuous random variable with support on a half-line such a..."""

def beta_prime_sample_batch(out: np.ndarray, alpha: float, beta: float, seed: int = 42) -> None:
    """Sample Beta prime into ``out`` (float64). Beta prime distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def burr_sample_batch(out: np.ndarray, c: float, scale: float, seed: int = 42) -> None:
    """Sample Burr into ``out`` (float64). Burr distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def chi_sample_batch(out: np.ndarray, df: float, seed: int = 42) -> None:
    """Sample Chi into ``out`` (float64). Chi distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def chi_squared_sample_batch(out: np.ndarray, df: float, seed: int = 42) -> None:
    """Sample Chi-squared into ``out`` (float64). Chi-squared distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def noncentral_chi_squared_sample_batch(out: np.ndarray, df: float, ncp: float, seed: int = 42) -> None:
    """Sample Noncentral chi-squared into ``out`` (float64). Noncentral chi-squared distribution: A continuous random variable with support on a half-line suc..."""

def inverse_chi_squared_sample_batch(out: np.ndarray, df: float, scale: float, seed: int = 42) -> None:
    """Sample Inverse chi-squared into ``out`` (float64). Inverse chi-squared distribution: A continuous random variable with support on a half-line such a..."""

def scaled_inverse_chi_squared_sample_batch(out: np.ndarray, df: float, scale: float, seed: int = 42) -> None:
    """Sample Scaled inverse chi-squared into ``out`` (float64). Scaled inverse chi-squared distribution: A continuous random variable with support on a half-line..."""

def dagum_sample_batch(out: np.ndarray, c: float, scale: float, seed: int = 42) -> None:
    """Sample Dagum into ``out`` (float64). Dagum distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def davis_sample_batch(out: np.ndarray, nu: float, seed: int = 42) -> None:
    """Sample Davis into ``out`` (float64). Davis distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def erlang_sample_batch(out: np.ndarray, a: float, scale: float, seed: int = 42) -> None:
    """Sample Erlang into ``out`` (float64). Erlang distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def hyper_sample_batch(out: np.ndarray, lambda1: float, lambda2: float, w1: float, seed: int = 42) -> None:
    """Sample Hyper into ``out`` (float64). Hyper distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def exponential_sample_batch(out: np.ndarray, rate: float, seed: int = 42) -> None:
    """Sample Exponential into ``out`` (float64). Waiting time until the next event when events occur at a constant rate."""

def hyperexponential_sample_batch(out: np.ndarray, lambda1: float, lambda2: float, w1: float, seed: int = 42) -> None:
    """Sample Hyperexponential into ``out`` (float64). Hyperexponential distribution: A continuous random variable with support on a half-line such as [..."""

def hypoexponential_sample_batch(out: np.ndarray, rate1: float, rate2: float, seed: int = 42) -> None:
    """Sample Hypoexponential into ``out`` (float64). Hypoexponential distribution: A continuous random variable with support on a half-line such as [0..."""

def exponential_logarithmic_sample_batch(out: np.ndarray, scale: float, seed: int = 42) -> None:
    """Sample Exponential-logarithmic into ``out`` (float64). Exponential-logarithmic distribution: A continuous random variable with support on a half-line su..."""

def f_sample_batch(out: np.ndarray, dfn: float, dfd: float, seed: int = 42) -> None:
    """Sample _F_ into ``out`` (float64). _F_ distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def noncentral_f_sample_batch(out: np.ndarray, dfn: float, dfd: float, nc: float, seed: int = 42) -> None:
    """Sample Noncentral F into ``out`` (float64). Noncentral F distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def folded_normal_sample_batch(out: np.ndarray, c: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Folded normal into ``out`` (float64). Folded normal distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def frechet_sample_batch(out: np.ndarray, c: float, scale: float, seed: int = 42) -> None:
    """Sample Fréchet into ``out`` (float64). Fréchet distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def gamma_sample_batch(out: np.ndarray, shape: float, scale: float, seed: int = 42) -> None:
    """Sample Gamma into ``out`` (float64). Gamma distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def generalized_gamma_sample_batch(out: np.ndarray, a: float, c: float, scale: float, seed: int = 42) -> None:
    """Sample Generalized gamma into ``out`` (float64). Generalized gamma distribution: A continuous random variable with support on a half-line such as..."""

def inverse_gamma_sample_batch(out: np.ndarray, shape: float, scale: float, seed: int = 42) -> None:
    """Sample Inverse gamma into ``out`` (float64). Inverse gamma distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def gamma_gompertz_sample_batch(out: np.ndarray, shape: float, scale: float, seed: int = 42) -> None:
    """Sample gamma/Gompertz into ``out`` (float64). gamma/Gompertz distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def gompertz_sample_batch(out: np.ndarray, c: float, scale: float, seed: int = 42) -> None:
    """Sample Gompertz into ``out`` (float64). Gompertz distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def shifted_sample_batch(out: np.ndarray, loc: float, rate: float, seed: int = 42) -> None:
    """Sample Shifted into ``out`` (float64). Shifted distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def half_logistic_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Half-logistic into ``out`` (float64). Half-logistic distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def half_normal_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Half-normal into ``out`` (float64). Half-normal distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def hotellings_t_squared_sample_batch(out: np.ndarray, df1: float, df2: float, seed: int = 42) -> None:
    """Sample Hotelling's _T_\-squared into ``out`` (float64). Hotelling's _T_\-squared distribution: A continuous random variable with support on a half-line s..."""

def hartmanwatson_sample_batch(out: np.ndarray, nu: float, seed: int = 42) -> None:
    """Sample Hartman–Watson into ``out`` (float64). Hartman–Watson distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def inverse_gaussian_sample_batch(out: np.ndarray, mu: float, lambda_: float, seed: int = 42) -> None:
    """Sample Inverse Gaussian into ``out`` (float64). Inverse Gaussian distribution: A continuous random variable with support on a half-line such as [..."""

def generalized_sample_batch(out: np.ndarray, a: float, c: float, scale: float, seed: int = 42) -> None:
    """Sample Generalized into ``out`` (float64). Generalized distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def kolmogorov_sample_batch(out: np.ndarray, scale: float, seed: int = 42) -> None:
    """Sample Kolmogorov into ``out`` (float64). Kolmogorov distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def levy_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Lévy into ``out`` (float64). Lévy distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def log_cauchy_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Log-Cauchy into ``out`` (float64). Log-Cauchy distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def log_laplace_sample_batch(out: np.ndarray, c: float, scale: float, seed: int = 42) -> None:
    """Sample Log-Laplace into ``out`` (float64). Log-Laplace distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def log_logistic_sample_batch(out: np.ndarray, c: float, scale: float, seed: int = 42) -> None:
    """Sample Log-logistic into ``out`` (float64). Log-logistic distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def log_normal_sample_batch(out: np.ndarray, mu: float, sigma: float, seed: int = 42) -> None:
    """Sample Log-normal into ``out`` (float64). Log-normal distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def log_t_sample_batch(out: np.ndarray, df: float, seed: int = 42) -> None:
    """Sample Log-t into ``out`` (float64). Log-t distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def lomax_sample_batch(out: np.ndarray, c: float, scale: float, seed: int = 42) -> None:
    """Sample Lomax into ``out`` (float64). Lomax distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def matrix_exponential_sample_batch(out: np.ndarray, rate1: float, rate2: float, rate3: float, seed: int = 42) -> None:
    """Sample Matrix-exponential into ``out`` (float64). Matrix-exponential distribution: A continuous random variable with support on a half-line such as..."""

def maxwellboltzmann_sample_batch(out: np.ndarray, a: float, seed: int = 42) -> None:
    """Sample Maxwell–Boltzmann into ``out`` (float64). Maxwell–Boltzmann distribution: A continuous random variable with support on a half-line such as..."""

def maxwelljuttner_sample_batch(out: np.ndarray, a: float, seed: int = 42) -> None:
    """Sample Maxwell–Jüttner into ``out`` (float64). Maxwell–Jüttner distribution: A continuous random variable with support on a half-line such as [0..."""

def mittag_leffler_sample_batch(out: np.ndarray, alpha: float, scale: float, seed: int = 42) -> None:
    """Sample Mittag-Leffler into ``out`` (float64). Mittag-Leffler distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def nakagami_sample_batch(out: np.ndarray, nu: float, scale: float, seed: int = 42) -> None:
    """Sample Nakagami into ``out`` (float64). Nakagami distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def pareto_sample_batch(out: np.ndarray, alpha: float, scale: float, seed: int = 42) -> None:
    """Sample Pareto into ``out`` (float64). Pareto distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def phase_type_sample_batch(out: np.ndarray, rate1: float, rate2: float, rate3: float, seed: int = 42) -> None:
    """Sample Phase-type into ``out`` (float64). Phase-type distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def poly_weibull_sample_batch(out: np.ndarray, shape1: float, shape2: float, weight: float, seed: int = 42) -> None:
    """Sample Poly-Weibull into ``out`` (float64). Poly-Weibull distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def rayleigh_sample_batch(out: np.ndarray, scale: float, seed: int = 42) -> None:
    """Sample Rayleigh into ``out`` (float64). Rayleigh distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def relativistic_breitwigner_sample_batch(out: np.ndarray, rho: float, seed: int = 42) -> None:
    """Sample Relativistic Breit–Wigner into ``out`` (float64). Relativistic Breit–Wigner distribution: A continuous random variable with support on a half-line..."""

def rice_sample_batch(out: np.ndarray, b: float, scale: float, seed: int = 42) -> None:
    """Sample Rice into ``out`` (float64). Rice distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def truncated_normal_sample_batch(out: np.ndarray, a: float, b: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Truncated normal into ``out`` (float64). Truncated normal distribution: A continuous random variable with support on a half-line such as [..."""

def type_2_gumbel_sample_batch(out: np.ndarray, shape: float, scale: float, seed: int = 42) -> None:
    """Sample type-2 Gumbel into ``out`` (float64). type-2 Gumbel distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def weibull_sample_batch(out: np.ndarray, shape: float, scale: float, seed: int = 42) -> None:
    """Sample Weibull into ``out`` (float64). Weibull distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def discrete_weibull_sample_batch(out: np.ndarray, c: float, seed: int = 42) -> None:
    """Sample Discrete Weibull into ``out`` (float64). Discrete Weibull distribution: A continuous random variable with support on a half-line such as [..."""

def wilkss_lambda_sample_batch(out: np.ndarray, a: float, b: float, seed: int = 42) -> None:
    """Sample Wilks's lambda into ``out`` (float64). Wilks's lambda distribution: A continuous random variable with support on a half-line such as [0,∞)."""

def cauchy_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Cauchy into ``out`` (float64). Cauchy distribution: A continuous random variable with support on the entire real line."""

def exponential_power_sample_batch(out: np.ndarray, scale: float, seed: int = 42) -> None:
    """Sample Exponential power into ``out`` (float64). Exponential power distribution: A continuous random variable with support on the entire real line."""

def fishers_z_sample_batch(out: np.ndarray, n_: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Fisher's _z_ into ``out`` (float64). Fisher's _z_ distribution: A continuous random variable with support on the entire real line."""

def kaniadakis_gaussian_sample_batch(out: np.ndarray, kappa: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Kaniadakis κ-Gaussian into ``out`` (float64). Kaniadakis κ-Gaussian distribution: A continuous random variable with support on the entire real..."""

def gaussian_q_sample_batch(out: np.ndarray, q: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Gaussian _q_ into ``out`` (float64). Gaussian _q_ distribution: A continuous random variable with support on the entire real line."""

def generalized_hyperbolic_sample_batch(out: np.ndarray, p: float, a: float, b: float, seed: int = 42) -> None:
    """Sample Generalized hyperbolic into ``out`` (float64). Generalized hyperbolic distribution: A continuous random variable with support on the entire real..."""

def generalized_logistic_logistic_beta_sample_batch(out: np.ndarray, c: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Generalized logistic (logistic-beta) into ``out`` (float64). Generalized logistic (logistic-beta) distribution: A continuous random variable with support on t..."""

def generalized_normal_sample_batch(out: np.ndarray, beta: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Generalized normal into ``out`` (float64). Generalized normal distribution: A continuous random variable with support on the entire real line."""

def geometric_stable_sample_batch(out: np.ndarray, alpha: float, beta: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Geometric stable into ``out`` (float64). Geometric stable distribution: A continuous random variable with support on the entire real line."""

def gumbel_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Gumbel into ``out`` (float64). Gumbel distribution: A continuous random variable with support on the entire real line."""

def holtsmark_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Holtsmark into ``out`` (float64). Holtsmark distribution: A continuous random variable with support on the entire real line."""

def hyperbolic_secant_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Hyperbolic secant into ``out`` (float64). Hyperbolic secant distribution: A continuous random variable with support on the entire real line."""

def johnsons_su_sample_batch(out: np.ndarray, a: float, b: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Johnson's _SU_ into ``out`` (float64). Johnson's _SU_ distribution: A continuous random variable with support on the entire real line."""

def landau_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Landau into ``out`` (float64). Landau distribution: A continuous random variable with support on the entire real line."""

def laplace_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Laplace into ``out`` (float64). Laplace distribution: A continuous random variable with support on the entire real line."""

def asymmetric_sample_batch(out: np.ndarray, kappa: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Asymmetric into ``out`` (float64). Asymmetric distribution: A continuous random variable with support on the entire real line."""

def logistic_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Logistic into ``out`` (float64). Logistic distribution: A continuous random variable with support on the entire real line."""

def noncentral_t_sample_batch(out: np.ndarray, df: float, nc: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Noncentral _t_ into ``out`` (float64). Noncentral _t_ distribution: A continuous random variable with support on the entire real line."""

def normal_sample_batch(out: np.ndarray, mu: float, sigma: float, seed: int = 42) -> None:
    """Sample Normal (Gaussian) into ``out`` (float64). The bell curve: symmetric uncertainty around a central value with controlled spread."""

def normal_inverse_gaussian_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Normal-inverse Gaussian into ``out`` (float64). Normal-inverse Gaussian distribution: A continuous random variable with support on the entire rea... Tier: heuristic."""

def skew_normal_sample_batch(out: np.ndarray, loc: float, scale: float, alpha: float, seed: int = 42) -> None:
    """Sample Skew normal into ``out`` (float64). Skew normal distribution: A continuous random variable with support on the entire real line."""

def slash_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Slash into ``out`` (float64). Slash distribution: A continuous random variable with support on the entire real line."""

def stable_sample_batch(out: np.ndarray, alpha: float, beta: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Stable into ``out`` (float64). Stable distribution: A continuous random variable with support on the entire real line."""

def students_t_sample_batch(out: np.ndarray, df: float, seed: int = 42) -> None:
    """Sample Student's _t_ into ``out`` (float64). Student's _t_ distribution: A continuous random variable with support on the entire real line."""

def tracywidom_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Tracy–Widom into ``out`` (float64). Tracy–Widom distribution: A continuous random variable with support on the entire real line."""

def variance_gamma_sample_batch(out: np.ndarray, a: float, b: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Variance-gamma into ``out`` (float64). Variance-gamma distribution: A continuous random variable with support on the entire real line."""

def voigt_sample_batch(out: np.ndarray, sigma: float, gamma_width: float, loc: float, seed: int = 42) -> None:
    """Sample Voigt into ``out`` (float64). Voigt distribution: A continuous random variable with support on the entire real line."""

def generalized_chi_squared_sample_batch(out: np.ndarray, df1: float, w1: float, df2: float, w2: float, seed: int = 42) -> None:
    """Sample Generalized chi-squared into ``out`` (float64). Generalized chi-squared distribution: A continuous distribution whose support type changes with p..."""

def generalized_extreme_value_sample_batch(out: np.ndarray, c: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Generalized extreme value into ``out`` (float64). Generalized extreme value distribution: A continuous distribution whose support type changes with..."""

def generalized_pareto_sample_batch(out: np.ndarray, c: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Generalized Pareto into ``out`` (float64). Generalized Pareto distribution: A continuous distribution whose support type changes with parame..."""

def marchenkopastur_sample_batch(out: np.ndarray, gamma_ratio: float, sigma: float, seed: int = 42) -> None:
    """Sample Marchenko–Pastur into ``out`` (float64). Marchenko–Pastur distribution: A continuous distribution whose support type changes with parameters."""

def kaniadakis_exponential_sample_batch(out: np.ndarray, kappa: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Kaniadakis _κ_\-exponential into ``out`` (float64). Kaniadakis _κ_\-exponential distribution: A continuous distribution whose support type changes wi..."""

def kaniadakis_gamma_sample_batch(out: np.ndarray, kappa: float, shape: float, scale: float, seed: int = 42) -> None:
    """Sample Kaniadakis _κ_\-Gamma into ``out`` (float64). Kaniadakis _κ_\-Gamma distribution: A continuous distribution whose support type changes with par..."""

def kaniadakis_weibull_sample_batch(out: np.ndarray, kappa: float, shape: float, scale: float, seed: int = 42) -> None:
    """Sample Kaniadakis _κ_\-Weibull into ``out`` (float64). Kaniadakis _κ_\-Weibull distribution: A continuous distribution whose support type changes with p..."""

def kaniadakis_logistic_sample_batch(out: np.ndarray, kappa: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Kaniadakis _κ_\-Logistic into ``out`` (float64). Kaniadakis _κ_\-Logistic distribution: A continuous distribution whose support type changes with..."""

def kaniadakis_erlang_sample_batch(out: np.ndarray, kappa: float, shape: float, scale: float, seed: int = 42) -> None:
    """Sample Kaniadakis _κ_\-Erlang into ``out`` (float64). Kaniadakis _κ_\-Erlang distribution: A continuous distribution whose support type changes with pa..."""

def q_exponential_sample_batch(out: np.ndarray, q: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample _q_\-exponential into ``out`` (float64). _q_\-exponential distribution: A continuous distribution whose support type changes with parameters."""

def q_gaussian_sample_batch(out: np.ndarray, q: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample _q_\-Gaussian into ``out`` (float64). _q_\-Gaussian distribution: A continuous distribution whose support type changes with parameters."""

def q_weibull_sample_batch(out: np.ndarray, q: float, shape: float, scale: float, seed: int = 42) -> None:
    """Sample _q_\-Weibull into ``out`` (float64). _q_\-Weibull distribution: A continuous distribution whose support type changes with parameters."""

def shifted_log_logistic_sample_batch(out: np.ndarray, scale: float, seed: int = 42) -> None:
    """Sample Shifted log-logistic into ``out`` (float64). Shifted log-logistic distribution: A continuous distribution whose support type changes with para..."""

def tukey_lambda_sample_batch(out: np.ndarray, lam: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Tukey lambda into ``out`` (float64). Tukey lambda distribution: A continuous distribution whose support type changes with parameters."""

def rectified_gaussian_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Rectified Gaussian into ``out`` (float64). Rectified Gaussian distribution: A distribution mixing continuous and discrete components. Tier: heuristic."""

def ewens_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Ewens into ``out`` (float64). Ewens distribution: A joint distribution over several discrete random variables. Tier: heuristic."""

def multinomial_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Multinomial into ``out`` (float64). Multinomial distribution: A joint distribution over several discrete random variables. Tier: heuristic."""

def dirichlet_multinomial_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Dirichlet-multinomial into ``out`` (float64). Dirichlet-multinomial distribution: A joint distribution over several discrete random variables. Tier: heuristic."""

def negative_multinomial_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Negative multinomial into ``out`` (float64). Negative multinomial distribution: A joint distribution over several discrete random variables. Tier: heuristic."""

def dirichlet_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Dirichlet into ``out`` (float64). Dirichlet distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def generalized_dirichlet_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Generalized Dirichlet into ``out`` (float64). Generalized Dirichlet distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def multivariate_laplace_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Multivariate Laplace into ``out`` (float64). Multivariate Laplace distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def multivariate_normal_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Multivariate normal into ``out`` (float64). Multivariate normal distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def multivariate_stable_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Multivariate stable into ``out`` (float64). Multivariate stable distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def multivariate_t_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Multivariate _t_ into ``out`` (float64). Multivariate _t_ distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def normal_gamma_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Normal-gamma into ``out`` (float64). Normal-gamma distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def normal_inverse_gamma_distribution_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Inverse gamma into ``out`` (float64). Inverse gamma distribution: A joint distribution over several continuous random variables. Tier: heuristic."""

def lkj_sample_batch(out: np.ndarray, eta: float, seed: int = 42) -> None:
    """Sample LKJ into ``out`` (float64). LKJ distribution: A distribution over random matrices (covariance-like objects)."""

def matrix_beta_sample_batch(out: np.ndarray, a: float, b: float, seed: int = 42) -> None:
    """Sample Matrix beta into ``out`` (float64). Matrix beta distribution: A distribution over random matrices (covariance-like objects)."""

def matrix_f_sample_batch(out: np.ndarray, df1: float, df2: float, v00: float, v11: float, seed: int = 42) -> None:
    """Sample Matrix _F_ into ``out`` (float64). Matrix _F_ distribution: A distribution over random matrices (covariance-like objects)."""

def matrix_normal_sample_batch(out: np.ndarray, row_var: float, col_var: float, seed: int = 42) -> None:
    """Sample Matrix normal into ``out`` (float64). Matrix normal distribution: A distribution over random matrices (covariance-like objects)."""

def matrix_t_sample_batch(out: np.ndarray, df: float, row_var: float, col_var: float, seed: int = 42) -> None:
    """Sample Matrix _t_ into ``out`` (float64). Matrix _t_ distribution: A distribution over random matrices (covariance-like objects)."""

def matrix_gamma_sample_batch(out: np.ndarray, shape: float, v00: float, v11: float, seed: int = 42) -> None:
    """Sample Matrix gamma into ``out`` (float64). Matrix gamma distribution: A distribution over random matrices (covariance-like objects)."""

def inverse_matrix_gamma_distribution_sample_batch(out: np.ndarray, shape: float, v00: float, v11: float, seed: int = 42) -> None:
    """Sample Inverse gamma into ``out`` (float64). Inverse gamma distribution: A distribution over random matrices (covariance-like objects)."""

def wishart_sample_batch(out: np.ndarray, df: float, v00: float, v01: float, v11: float, seed: int = 42) -> None:
    """Sample Wishart into ``out`` (float64). Wishart distribution: A distribution over random matrices (covariance-like objects)."""

def normal_wishart_sample_batch(out: np.ndarray, df: float, kappa: float, v00: float, v11: float, seed: int = 42) -> None:
    """Sample Normal-Wishart into ``out`` (float64). Normal-Wishart distribution: A distribution over random matrices (covariance-like objects)."""

def inverse_wishart_sample_batch(out: np.ndarray, df: float, v00: float, v01: float, v11: float, seed: int = 42) -> None:
    """Sample Inverse-Wishart into ``out`` (float64). Inverse-Wishart distribution: A distribution over random matrices (covariance-like objects)."""

def normal_inverse_sample_batch(out: np.ndarray, df: float, kappa: float, v00: float, v11: float, seed: int = 42) -> None:
    """Sample Normal-inverse into ``out`` (float64). Normal-inverse distribution: A distribution over random matrices (covariance-like objects)."""

def complex_sample_batch(out: np.ndarray, df: float, v00: float, v11: float, seed: int = 42) -> None:
    """Sample Complex into ``out`` (float64). Complex distribution: A distribution over random matrices (covariance-like objects)."""

def uniform_distribution_on_a_stiefel_manifold_sample_batch(out: np.ndarray, seed: int = 42) -> None:
    """Sample Uniform distribution on a Stiefel manifold into ``out`` (float64). Uniform distribution on a Stiefel manifold distribution: A distribution over random matrices (cov..."""

def circular_uniform_sample_batch(out: np.ndarray, mu: float, kappa: float, seed: int = 42) -> None:
    """Sample Circular uniform into ``out`` (float64). Circular uniform distribution: A distribution for angles or directions on the circle."""

def univariate_von_mises_sample_batch(out: np.ndarray, mu: float, kappa: float, seed: int = 42) -> None:
    """Sample Univariate von Mises into ``out`` (float64). Univariate von Mises distribution: A distribution for angles or directions on the circle."""

def wrapped_normal_sample_batch(out: np.ndarray, mu: float, sigma: float, seed: int = 42) -> None:
    """Sample Wrapped normal into ``out`` (float64). Wrapped normal distribution: A distribution for angles or directions on the circle."""

def wrapped_cauchy_sample_batch(out: np.ndarray, c: float, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Wrapped Cauchy into ``out`` (float64). Wrapped Cauchy distribution: A distribution for angles or directions on the circle."""

def wrapped_exponential_sample_batch(out: np.ndarray, loc: float, rate: float, seed: int = 42) -> None:
    """Sample Wrapped exponential into ``out`` (float64). Wrapped exponential distribution: A distribution for angles or directions on the circle."""

def wrapped_asymmetric_laplace_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Wrapped asymmetric Laplace into ``out`` (float64). Wrapped asymmetric Laplace distribution: A distribution for angles or directions on the circle."""

def wrapped_levy_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Wrapped Lévy into ``out`` (float64). Wrapped Lévy distribution: A distribution for angles or directions on the circle."""

def kent_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Kent into ``out`` (float64). Kent distribution: A bivariate distribution on the sphere. Tier: heuristic."""

def bivariate_von_mises_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Bivariate von Mises into ``out`` (float64). Bivariate von Mises distribution: A bivariate distribution on a torus. Tier: heuristic."""

def von_misesfisher_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample von Mises–Fisher into ``out`` (float64). von Mises–Fisher distribution: A multivariate directional distribution on hyperspheres. Tier: heuristic."""

def bingham_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Bingham into ``out`` (float64). Bingham distribution: A multivariate directional distribution on hyperspheres. Tier: heuristic."""

def dirac_delta_function_sample_batch(out: np.ndarray, x0: float, seed: int = 42) -> None:
    """Sample Dirac delta function into ``out`` (float64). Dirac delta function: A distribution with all probability mass at a single point."""

def cantor_sample_batch(out: np.ndarray, loc: float, scale: float, seed: int = 42) -> None:
    """Sample Cantor into ``out`` (float64). Cantor distribution: A singular distribution supported on a fractal or lower-dimensional set. Tier: heuristic."""

__all__: list[str] = [
    "benford_sample_batch",
    "bernoulli_sample_batch",
    "beta_binomial_sample_batch",
    "binomial_sample_batch",
    "categorical_sample_batch",
    "hypergeometric_sample_batch",
    "negative_hypergeometric_sample_batch",
    "poisson_binomial_sample_batch",
    "rademacher_sample_batch",
    "soliton_sample_batch",
    "discrete_uniform_sample_batch",
    "zipf_sample_batch",
    "zipf_mandelbrot_sample_batch",
    "beta_negative_binomial_sample_batch",
    "borel_sample_batch",
    "conwaymaxwellpoisson_sample_batch",
    "discrete_phase_type_sample_batch",
    "delaporte_sample_batch",
    "extended_negative_binomial_sample_batch",
    "floryschulz_sample_batch",
    "gausskuzmin_sample_batch",
    "geometric_sample_batch",
    "logarithmic_sample_batch",
    "mixed_poisson_sample_batch",
    "negative_binomial_sample_batch",
    "panjer_sample_batch",
    "parabolic_fractal_sample_batch",
    "poisson_sample_batch",
    "skellam_sample_batch",
    "yulesimon_sample_batch",
    "zeta_sample_batch",
    "arcsine_sample_batch",
    "argus_sample_batch",
    "baldingnichols_sample_batch",
    "bates_sample_batch",
    "beta_sample_batch",
    "generalized_beta_sample_batch",
    "beta_rectangular_sample_batch",
    "continuous_bernoulli_sample_batch",
    "continuous_binomial_sample_batch",
    "irwinhall_sample_batch",
    "kumaraswamy_sample_batch",
    "logit_normal_sample_batch",
    "noncentral_beta_sample_batch",
    "pert_sample_batch",
    "raised_cosine_sample_batch",
    "reciprocal_sample_batch",
    "triangular_sample_batch",
    "u_quadratic_sample_batch",
    "uniform_sample_batch",
    "wigner_semicircle_sample_batch",
    "benini_sample_batch",
    "benktander_1st_kind_sample_batch",
    "benktander_2nd_kind_sample_batch",
    "beta_prime_sample_batch",
    "burr_sample_batch",
    "chi_sample_batch",
    "chi_squared_sample_batch",
    "noncentral_chi_squared_sample_batch",
    "inverse_chi_squared_sample_batch",
    "scaled_inverse_chi_squared_sample_batch",
    "dagum_sample_batch",
    "davis_sample_batch",
    "erlang_sample_batch",
    "hyper_sample_batch",
    "exponential_sample_batch",
    "hyperexponential_sample_batch",
    "hypoexponential_sample_batch",
    "exponential_logarithmic_sample_batch",
    "f_sample_batch",
    "noncentral_f_sample_batch",
    "folded_normal_sample_batch",
    "frechet_sample_batch",
    "gamma_sample_batch",
    "generalized_gamma_sample_batch",
    "inverse_gamma_sample_batch",
    "gamma_gompertz_sample_batch",
    "gompertz_sample_batch",
    "shifted_sample_batch",
    "half_logistic_sample_batch",
    "half_normal_sample_batch",
    "hotellings_t_squared_sample_batch",
    "hartmanwatson_sample_batch",
    "inverse_gaussian_sample_batch",
    "generalized_sample_batch",
    "kolmogorov_sample_batch",
    "levy_sample_batch",
    "log_cauchy_sample_batch",
    "log_laplace_sample_batch",
    "log_logistic_sample_batch",
    "log_normal_sample_batch",
    "log_t_sample_batch",
    "lomax_sample_batch",
    "matrix_exponential_sample_batch",
    "maxwellboltzmann_sample_batch",
    "maxwelljuttner_sample_batch",
    "mittag_leffler_sample_batch",
    "nakagami_sample_batch",
    "pareto_sample_batch",
    "phase_type_sample_batch",
    "poly_weibull_sample_batch",
    "rayleigh_sample_batch",
    "relativistic_breitwigner_sample_batch",
    "rice_sample_batch",
    "truncated_normal_sample_batch",
    "type_2_gumbel_sample_batch",
    "weibull_sample_batch",
    "discrete_weibull_sample_batch",
    "wilkss_lambda_sample_batch",
    "cauchy_sample_batch",
    "exponential_power_sample_batch",
    "fishers_z_sample_batch",
    "kaniadakis_gaussian_sample_batch",
    "gaussian_q_sample_batch",
    "generalized_hyperbolic_sample_batch",
    "generalized_logistic_logistic_beta_sample_batch",
    "generalized_normal_sample_batch",
    "geometric_stable_sample_batch",
    "gumbel_sample_batch",
    "holtsmark_sample_batch",
    "hyperbolic_secant_sample_batch",
    "johnsons_su_sample_batch",
    "landau_sample_batch",
    "laplace_sample_batch",
    "asymmetric_sample_batch",
    "logistic_sample_batch",
    "noncentral_t_sample_batch",
    "normal_sample_batch",
    "normal_inverse_gaussian_sample_batch",
    "skew_normal_sample_batch",
    "slash_sample_batch",
    "stable_sample_batch",
    "students_t_sample_batch",
    "tracywidom_sample_batch",
    "variance_gamma_sample_batch",
    "voigt_sample_batch",
    "generalized_chi_squared_sample_batch",
    "generalized_extreme_value_sample_batch",
    "generalized_pareto_sample_batch",
    "marchenkopastur_sample_batch",
    "kaniadakis_exponential_sample_batch",
    "kaniadakis_gamma_sample_batch",
    "kaniadakis_weibull_sample_batch",
    "kaniadakis_logistic_sample_batch",
    "kaniadakis_erlang_sample_batch",
    "q_exponential_sample_batch",
    "q_gaussian_sample_batch",
    "q_weibull_sample_batch",
    "shifted_log_logistic_sample_batch",
    "tukey_lambda_sample_batch",
    "rectified_gaussian_sample_batch",
    "ewens_sample_batch",
    "multinomial_sample_batch",
    "dirichlet_multinomial_sample_batch",
    "negative_multinomial_sample_batch",
    "dirichlet_sample_batch",
    "generalized_dirichlet_sample_batch",
    "multivariate_laplace_sample_batch",
    "multivariate_normal_sample_batch",
    "multivariate_stable_sample_batch",
    "multivariate_t_sample_batch",
    "normal_gamma_sample_batch",
    "normal_inverse_gamma_distribution_sample_batch",
    "lkj_sample_batch",
    "matrix_beta_sample_batch",
    "matrix_f_sample_batch",
    "matrix_normal_sample_batch",
    "matrix_t_sample_batch",
    "matrix_gamma_sample_batch",
    "inverse_matrix_gamma_distribution_sample_batch",
    "wishart_sample_batch",
    "normal_wishart_sample_batch",
    "inverse_wishart_sample_batch",
    "normal_inverse_sample_batch",
    "complex_sample_batch",
    "uniform_distribution_on_a_stiefel_manifold_sample_batch",
    "circular_uniform_sample_batch",
    "univariate_von_mises_sample_batch",
    "wrapped_normal_sample_batch",
    "wrapped_cauchy_sample_batch",
    "wrapped_exponential_sample_batch",
    "wrapped_asymmetric_laplace_sample_batch",
    "wrapped_levy_sample_batch",
    "kent_sample_batch",
    "bivariate_von_mises_sample_batch",
    "von_misesfisher_sample_batch",
    "bingham_sample_batch",
    "dirac_delta_function_sample_batch",
    "cantor_sample_batch",
]
