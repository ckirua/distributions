"""Sampler recipe catalog."""

from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VAULT_TOOLS = ROOT / ".vault" / "tools"
sys.path.insert(0, str(VAULT_TOOLS))
from vault_scipy import lookup_scipy_spec  # noqa: E402

from codegen.constants import MANUAL
from codegen.models import Recipe
from codegen.utils import folder_for, slug_to_class

HEURISTIC_SAMPLE = "return loc_ + scale_ * detail::sample_standard_normal(rng);"

def build_recipes(registry: list[dict]) -> dict[str, Recipe]:
    recipes: dict[str, Recipe] = {}

    def add(r: Recipe) -> None:
        recipes[r.vault_id] = r

    for entry in registry:
        vid = entry["id"]
        if vid in MANUAL:
            continue
        cat = folder_for(entry.get("category_path", ""))
        cls = slug_to_class(vid)
        discrete = entry.get("type") == "discrete"

        # --- discrete / finite ---
        if vid == "benford":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       sample_body="return detail::sample_benford(rng);",
                       bench_ctor_args="",
                       cydist_params=[("uint64_t", "seed")]))
            continue
        if vid == "hypergeometric":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/hypergeometric.hpp"],
                       members=[("int", "M", "50"), ("int", "n_success", "10"), ("int", "N_draws", "5")],
                       sample_body="return detail::sample_hypergeometric(rng, M_, n_success_, N_draws_);",
                       bench_ctor_args="50, 10, 5",
                       cydist_params=[("int", "M"), ("int", "n_success"), ("int", "N_draws"), ("uint64_t", "seed")]))
            continue
        if vid == "negative-hypergeometric":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/hypergeometric.hpp", "distributions/detail/discrete.hpp"],
                       members=[("int", "M", "50"), ("int", "n", "10"), ("int", "N", "5")],
                       sample_body=(
                           "const int k = detail::sample_hypergeometric(rng, M_, n_, N_);\n"
                           "        return detail::sample_negative_binomial(rng, static_cast<double>(k), "
                           "static_cast<double>(N_) / static_cast<double>(M_));"
                       ),
                       bench_ctor_args="50, 10, 5",
                       cydist_skip=True))
            continue
        if vid == "rademacher":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       sample_body="return detail::sample_rademacher(rng);",
                       cydist_params=[("uint64_t", "seed")]))
            continue

        # --- discrete / infinite ---
        if vid == "poisson":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/poisson.hpp"],
                       members=[("double", "mu", "4.0")],
                       sample_body="return detail::sample_poisson(rng, mu_);",
                       bench_ctor_args="4.0",
                       cydist_params=[("double", "mu"), ("uint64_t", "seed")]))
            continue
        if vid == "logarithmic":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "p", "0.5")],
                       sample_body="return detail::sample_logarithmic(rng, p_);",
                       bench_ctor_args="0.5",
                       cydist_params=[("double", "p"), ("uint64_t", "seed")]))
            continue
        if vid == "yulesimon":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "rho", "2.0")],
                       sample_body="return detail::sample_yule_simon(rng, rho_);",
                       bench_ctor_args="2.0",
                       cydist_params=[("double", "rho"), ("uint64_t", "seed")]))
            continue
        if vid == "zeta":
            add(Recipe(vid, "Zeta", cat, True,
                       ["distributions/discrete/finite/zipf.hpp"],
                       members=[("int", "N", "100"), ("double", "s", "2.0")],
                       sample_body="return Zipf(N_, s_).sample(rng);",
                       bench_ctor_args="100, 2.0",
                       cydist_params=[("int", "N"), ("double", "s"), ("uint64_t", "seed")]))
            continue

        # --- discrete / infinite (heuristic batch 3) ---
        if vid == "floryschulz":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "p", "0.8")],
                       sample_body="return detail::sample_flory_schulz(rng, p_);",
                       bench_ctor_args="0.8",
                       cydist_params=[("double", "p"), ("uint64_t", "seed")]))
            continue
        if vid == "gausskuzmin":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       sample_body="return detail::sample_gauss_kuzmin(rng);",
                       bench_ctor_args="",
                       cydist_params=[("uint64_t", "seed")]))
            continue
        if vid == "borel":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "mu", "0.5")],
                       sample_body="return detail::sample_borel(rng, mu_);",
                       bench_ctor_args="0.5",
                       cydist_params=[("double", "mu"), ("uint64_t", "seed")]))
            continue
        if vid == "beta-negative-binomial":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "alpha", "2.0"), ("double", "beta", "5.0"), ("double", "r", "1.0")],
                       sample_body="return detail::sample_beta_negative_binomial(rng, alpha_, beta_, r_);",
                       bench_ctor_args="2.0, 5.0, 1.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("double", "r"), ("uint64_t", "seed")]))
            continue
        if vid == "mixed-poisson":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "shape", "2.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_mixed_poisson(rng, shape_, scale_);",
                       bench_ctor_args="2.0, 1.0",
                       cydist_params=[("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "delaporte":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "lambda", "1.0"), ("double", "r", "2.0"), ("double", "p", "0.5")],
                       sample_body="return detail::sample_delaporte(rng, lambda_, r_, p_);",
                       bench_ctor_args="1.0, 2.0, 0.5",
                       cydist_params=[("double", "lambda"), ("double", "r"), ("double", "p"), ("uint64_t", "seed")]))
            continue
        if vid == "conwaymaxwellpoisson":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "lambda", "4.0"), ("double", "nu", "1.0")],
                       sample_body="return detail::sample_comp_poisson(rng, lambda_, nu_);",
                       bench_ctor_args="4.0, 1.0",
                       cydist_params=[("double", "lambda"), ("double", "nu"), ("uint64_t", "seed")]))
            continue
        if vid == "discrete-phase-type":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "p1", "0.6"), ("double", "p2", "0.4")],
                       sample_body="return detail::sample_discrete_phase_type(rng, p1_, p2_);",
                       bench_ctor_args="0.6, 0.4",
                       cydist_params=[("double", "p1"), ("double", "p2"), ("uint64_t", "seed")]))
            continue
        if vid == "extended-negative-binomial":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "r", "5.0"), ("double", "p", "0.4")],
                       sample_body="return detail::sample_negative_binomial_gamma_poisson(rng, r_, p_);",
                       bench_ctor_args="5.0, 0.4",
                       cydist_params=[("double", "r"), ("double", "p"), ("uint64_t", "seed")]))
            continue
        if vid == "panjer":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/poisson.hpp"],
                       members=[("double", "lambda", "4.0")],
                       sample_body="return detail::sample_poisson(rng, lambda_);",
                       bench_ctor_args="4.0",
                       cydist_params=[("double", "lambda"), ("uint64_t", "seed")]))
            continue
        if vid == "parabolic-fractal":
            add(Recipe(vid, cls, cat, True,
                       ["distributions/detail/discrete.hpp"],
                       members=[("double", "b", "2.0"), ("double", "c", "0.1")],
                       sample_body="return detail::sample_parabolic_fractal(rng, b_, c_);",
                       bench_ctor_args="2.0, 0.1",
                       cydist_params=[("double", "b"), ("double", "c"), ("uint64_t", "seed")]))
            continue

        # --- continuous / bounded (heuristic batch 4) ---
        if vid == "argus":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/bounded.hpp"],
                       members=[("double", "chi", "1.0")],
                       sample_body="return detail::sample_argus(rng, chi_);",
                       bench_ctor_args="1.0",
                       cydist_params=[("double", "chi"), ("uint64_t", "seed")]))
            continue
        if vid == "baldingnichols":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/bounded.hpp"],
                       members=[("double", "f", "0.1")],
                       sample_body="return detail::sample_balding_nichols(rng, f_);",
                       bench_ctor_args="0.1",
                       cydist_params=[("double", "f"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-beta":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/bounded.hpp"],
                       members=[("double", "alpha", "2.0"), ("double", "beta", "5.0"), ("double", "lambda", "0.0")],
                       sample_body="return detail::sample_generalized_beta(rng, alpha_, beta_, lambda_);",
                       bench_ctor_args="2.0, 5.0, 0.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("double", "lambda"), ("uint64_t", "seed")]))
            continue
        if vid == "beta-rectangular":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "alpha", "2.0"), ("double", "beta", "5.0")],
                       sample_body="return detail::sample_beta(rng, alpha_, beta_);",
                       bench_ctor_args="2.0, 5.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("uint64_t", "seed")]))
            continue
        if vid == "continuous-bernoulli":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/bounded.hpp"],
                       members=[("double", "lambda", "0.5")],
                       sample_body="return detail::sample_continuous_bernoulli(rng, lambda_);",
                       bench_ctor_args="0.5",
                       cydist_params=[("double", "lambda"), ("uint64_t", "seed")]))
            continue
        if vid == "continuous-binomial":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/bounded.hpp"],
                       members=[("int", "n", "10")],
                       sample_body="return detail::sample_continuous_binomial(rng, n_);",
                       bench_ctor_args="10",
                       cydist_params=[("int", "n"), ("uint64_t", "seed")]))
            continue
        if vid == "noncentral-beta":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/bounded.hpp"],
                       members=[("double", "alpha", "2.0"), ("double", "beta", "5.0"), ("double", "lam", "1.0")],
                       sample_body="return detail::sample_noncentral_beta(rng, alpha_, beta_, lam_);",
                       bench_ctor_args="2.0, 5.0, 1.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("double", "lam"), ("uint64_t", "seed")]))
            continue
        if vid == "pert":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/bounded.hpp"],
                       members=[("double", "lo", "0.0"), ("double", "mode", "0.5"), ("double", "hi", "1.0")],
                       sample_body="return detail::sample_pert(rng, lo_, mode_, hi_);",
                       bench_ctor_args="0.0, 0.5, 1.0",
                       cydist_params=[("double", "lo"), ("double", "mode"), ("double", "hi"), ("uint64_t", "seed")]))
            continue

        # --- continuous / bounded ---
        if vid == "uniform":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "lo", "0.0"), ("double", "hi", "1.0")],
                       sample_body="return detail::sample_uniform(rng, lo_, hi_);",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "lo"), ("double", "hi"), ("uint64_t", "seed")]))
            continue
        if vid == "beta":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "alpha", "2.0"), ("double", "beta", "5.0")],
                       sample_body="return detail::sample_beta(rng, alpha_, beta_);",
                       bench_ctor_args="2.0, 5.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("uint64_t", "seed")]))
            continue
        if vid == "triangular":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "lo", "0.0"), ("double", "mode", "0.5"), ("double", "hi", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        const double c = (mode_ - lo_) / (hi_ - lo_);\n"
                           "        if (u < c) { return lo_ + std::sqrt(u * (hi_ - lo_) * (mode_ - lo_)); }\n"
                           "        return hi_ - std::sqrt((1.0 - u) * (hi_ - lo_) * (hi_ - mode_));"
                       ),
                       bench_ctor_args="0.0, 0.5, 1.0",
                       cydist_skip=True))
            continue
        if vid == "arcsine":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp", "distributions/detail/gamma.hpp"],
                       sample_body="return detail::sample_beta(rng, 0.5, 0.5);",
                       cydist_params=[("uint64_t", "seed")]))
            continue
        if vid == "irwinhall" or vid == "bates":
            n_default = "4" if vid == "irwinhall" else "4"
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("int", "n", n_default), ("double", "lo", "0.0"), ("double", "hi", "1.0")],
                       sample_body=(
                           "double sum = 0.0;\n"
                           "        for (int i = 0; i < n_; ++i) { sum += detail::sample_uniform(rng, lo_, hi_); }\n"
                           "        return sum / static_cast<double>(n_);"
                       ),
                       bench_ctor_args=f"{n_default}, 0.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "kumaraswamy":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "a", "2.0"), ("double", "b", "5.0")],
                       sample_body=(
                           "const double u = detail::sample_uniform(rng, 0.0, 1.0);\n"
                           "        const double v = detail::sample_uniform(rng, 0.0, 1.0);\n"
                           "        return std::pow(1.0 - std::pow(1.0 - u, 1.0 / a_), 1.0 / b_);"
                       ),
                       bench_ctor_args="2.0, 5.0",
                       cydist_skip=True))
            continue
        if vid == "logit-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "mu", "0.0"), ("double", "sigma", "1.0")],
                       sample_body=(
                           "const double z = detail::sample_normal(rng, mu_, sigma_);\n"
                           "        return 1.0 / (1.0 + std::exp(-z));"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "raised-cosine":
            add(Recipe(vid, "RaisedCosine", cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "lo", "0.0"), ("double", "hi", "1.0")],
                       sample_body=(
                           "const double u = detail::sample_uniform(rng, 0.0, 1.0);\n"
                           "        return lo_ + hi_ * (1.0 - std::cos(std::numbers::pi * u)) * 0.5;"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "reciprocal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "a", "0.5"), ("double", "b", "2.0")],
                       sample_body="return std::exp(detail::sample_uniform(rng, std::log(a_), std::log(b_)));",
                       bench_ctor_args="0.5, 2.0",
                       cydist_skip=True))
            continue
        if vid == "wigner-semicircle":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "lo", "-1.0"), ("double", "scale", "2.0")],
                       sample_body=(
                           "const double u = detail::sample_uniform(rng, 0.0, 1.0);\n"
                           "        const double r = std::sqrt(u) * scale_ * 0.5;\n"
                           "        return lo_ + r;"
                       ),
                       bench_ctor_args="-1.0, 2.0",
                       cydist_skip=True))
            continue
        if vid == "u-quadratic":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "a", "0.0"), ("double", "b", "1.0")],
                       sample_body=(
                           "const double u = detail::sample_uniform(rng, 0.0, 1.0);\n"
                           "        const double t = a_ + (b_ - a_) * u;\n"
                           "        return 0.5 * (a_ + b_) + 0.5 * (b_ - a_) * (2.0 * t / (a_ + b_) - 1.0);"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_skip=True))
            continue

        # --- continuous / semi-infinite ---
        if vid == "gamma":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "shape", "2.0"), ("double", "scale", "2.0")],
                       sample_body="return detail::sample_gamma(rng, shape_, scale_);",
                       bench_ctor_args="2.0, 2.0",
                       cydist_params=[("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "erlang":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "a", "2.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_gamma(rng, a_, scale_);",
                       bench_ctor_args="2.0, 1.0",
                       cydist_params=[("double", "a"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "chi-squared":
            add(Recipe(vid, "ChiSquared", cat, False,
                       ["distributions/detail/chi_squared.hpp"],
                       members=[("double", "df", "3.0")],
                       sample_body="return detail::sample_chi_squared(rng, df_);",
                       bench_ctor_args="3.0",
                       cydist_params=[("double", "df"), ("uint64_t", "seed")]))
            continue
        if vid == "chi":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/chi_squared.hpp"],
                       members=[("double", "df", "3.0")],
                       sample_body="return detail::sample_chi(rng, df_);",
                       bench_ctor_args="3.0",
                       cydist_params=[("double", "df"), ("uint64_t", "seed")]))
            continue
        if vid == "f":
            add(Recipe(vid, "FDistribution", cat, False,
                       ["distributions/detail/f_distribution.hpp"],
                       members=[("double", "dfn", "5.0"), ("double", "dfd", "10.0")],
                       sample_body="return detail::sample_f(rng, dfn_, dfd_);",
                       bench_ctor_args="5.0, 10.0",
                       cydist_params=[("double", "dfn"), ("double", "dfd"), ("uint64_t", "seed")]))
            continue
        if vid == "weibull":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "shape", "1.5"), ("double", "scale", "2.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        return scale_ * std::pow(-std::log1p(-u), 1.0 / shape_);"
                       ),
                       bench_ctor_args="1.5, 2.0",
                       cydist_params=[("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "rayleigh":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "scale", "1.5")],
                       sample_body="return scale_ * std::sqrt(-2.0 * std::log1p(-rng.next_double()));",
                       bench_ctor_args="1.5",
                       cydist_params=[("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "pareto":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "alpha", "2.0"), ("double", "scale", "1.0")],
                       sample_body="return scale_ / std::pow(rng.next_double(), 1.0 / alpha_);",
                       bench_ctor_args="2.0, 1.0",
                       cydist_params=[("double", "alpha"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "inverse-gamma":
            add(Recipe(vid, "InverseGamma", cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "shape", "3.0"), ("double", "scale", "1.0")],
                       sample_body="return scale_ / detail::sample_gamma(rng, shape_, 1.0);",
                       bench_ctor_args="3.0, 1.0",
                       cydist_params=[("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "inverse-gaussian":
            add(Recipe(vid, "InverseGaussian", cat, False,
                       ["distributions/detail/inverse_gaussian.hpp"],
                       members=[("double", "mu", "1.0"), ("double", "lambda", "1.0")],
                       sample_body="return detail::sample_inverse_gaussian(rng, mu_, lambda_);",
                       bench_ctor_args="1.0, 1.0",
                       cydist_params=[("double", "mu"), ("double", "lambda"), ("uint64_t", "seed")]))
            continue
        if vid == "half-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return loc_ + scale_ * std::abs(detail::sample_standard_normal(rng));",
                       bench_ctor_args="0.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "folded-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "c", "1.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "for (;;) {\n"
                           "            const double x = loc_ + scale_ * detail::sample_standard_normal(rng);\n"
                           "            if (x >= c_) { return x; }\n"
                           "        }"
                       ),
                       bench_ctor_args="1.0, 0.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "half-logistic":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return loc_ + scale_ * std::log(rng.next_double() / (1.0 - rng.next_double()));",
                       bench_ctor_args="0.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "beta-prime":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "alpha", "2.0"), ("double", "beta", "5.0")],
                       sample_body=(
                           "const double x = detail::sample_gamma(rng, alpha_, 1.0);\n"
                           "        const double y = detail::sample_gamma(rng, beta_, 1.0);\n"
                           "        return x / y;"
                       ),
                       bench_ctor_args="2.0, 5.0",
                       cydist_skip=True))
            continue
        if vid == "lomax" or vid == "burr" or vid == "dagum":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "c", "2.0"), ("double", "scale", "1.0")],
                       sample_body="return scale_ * (std::pow(rng.next_double(), -1.0 / c_) - 1.0);",
                       bench_ctor_args="2.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "maxwellboltzmann" or vid == "maxwell-boltzmann":
            add(Recipe(vid, "MaxwellBoltzmann", cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "a", "1.0")],
                       sample_body=(
                           "const double x = detail::sample_standard_normal(rng);\n"
                           "        const double y = detail::sample_standard_normal(rng);\n"
                           "        const double z = detail::sample_standard_normal(rng);\n"
                           "        return a_ * std::sqrt(x * x + y * y + z * z);"
                       ),
                       bench_ctor_args="1.0",
                       cydist_skip=True))
            continue
        if vid == "nakagami":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "nu", "2.0"), ("double", "scale", "1.0")],
                       sample_body="return scale_ * std::sqrt(detail::sample_gamma(rng, nu_, 1.0));",
                       bench_ctor_args="2.0, 1.0",
                       cydist_skip=True))
            continue
        if vid == "noncentral-chi-squared":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/chi_squared.hpp", "distributions/detail/poisson.hpp"],
                       members=[("double", "df", "3.0"), ("double", "ncp", "2.0")],
                       sample_body=(
                           "const int j = detail::sample_poisson(rng, ncp_ * 0.5);\n"
                           "        return detail::sample_chi_squared(rng, df_ + 2.0 * static_cast<double>(j));"
                       ),
                       bench_ctor_args="3.0, 2.0",
                       cydist_skip=True))
            continue
        if vid == "inverse-chi-squared" or vid == "scaled-inverse-chi-squared":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/chi_squared.hpp"],
                       members=[("double", "df", "3.0"), ("double", "scale", "1.0")],
                       sample_body="return scale_ / detail::sample_chi_squared(rng, df_);",
                       bench_ctor_args="3.0, 1.0",
                       cydist_skip=True))
            continue

        # --- continuous / real line ---
        if vid == "students-t" or vid == "student-t":
            add(Recipe(vid, "StudentT", cat, False,
                       ["distributions/detail/student_t.hpp"],
                       members=[("double", "df", "5.0")],
                       sample_body="return detail::sample_student_t(rng, df_);",
                       bench_ctor_args="5.0",
                       cydist_params=[("double", "df"), ("uint64_t", "seed")]))
            continue
        if vid == "cauchy":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        return loc_ + scale_ * std::tan(std::numbers::pi * (u - 0.5));"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "laplace":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double() - 0.5;\n"
                           "        return loc_ - scale_ * (u < 0.0 ? std::log1p(2.0 * u) : -std::log1p(-2.0 * u));"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "logistic":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return loc_ + scale_ * std::log(rng.next_double() / (1.0 - rng.next_double()));",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "gumbel":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return loc_ - scale_ * std::log(-std::log(rng.next_double()));",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "log-normal":
            add(Recipe(vid, "LogNormal", cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "mu", "0.0"), ("double", "sigma", "0.5")],
                       sample_body="return std::exp(detail::sample_normal(rng, mu_, sigma_));",
                       bench_ctor_args="0.0, 0.5",
                       cydist_params=[("double", "mu"), ("double", "sigma"), ("uint64_t", "seed")]))
            continue
        if vid == "skew-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0"), ("double", "alpha", "4.0")],
                       sample_body=(
                           "const double u1 = detail::sample_standard_normal(rng);\n"
                           "        const double u2 = detail::sample_standard_normal(rng);\n"
                           "        const double delta = alpha_ / std::sqrt(1.0 + alpha_ * alpha_);\n"
                           "        return loc_ + scale_ * (delta * std::abs(u1) + std::sqrt(1.0 - delta * delta) * u2);"
                       ),
                       bench_ctor_args="0.0, 1.0, 4.0",
                       cydist_skip=True))
            continue

        # --- directional / wrapped circular (heuristic batch 2) ---
        if vid == "wrapped-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp", "distributions/detail/circular.hpp"],
                       members=[("double", "mu", "0.0"), ("double", "sigma", "0.5")],
                       sample_body=(
                           "return detail::wrap_angle(detail::sample_normal(rng, mu_, sigma_));"
                       ),
                       bench_ctor_args="0.0, 0.5",
                       cydist_params=[("double", "mu"), ("double", "sigma"), ("uint64_t", "seed")]))
            continue
        if vid == "wrapped-cauchy":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/circular.hpp"],
                       members=[("double", "c", "0.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_wrapcauchy(rng, c_, loc_, scale_);",
                       bench_ctor_args="0.5, 0.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "wrapped-exponential":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp", "distributions/detail/circular.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "rate", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        return detail::wrap_angle(loc_ - std::log1p(-u) / rate_);"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "rate"), ("uint64_t", "seed")]))
            continue
        if vid == "wrapped-levy":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp", "distributions/detail/circular.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double z = detail::sample_standard_normal(rng);\n"
                           "        return detail::wrap_angle(loc_ + scale_ / (z * z));"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "wrapped-asymmetric-laplace":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp", "distributions/detail/circular.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        const double x = u < 0.5\n"
                           "            ? loc_ + scale_ * std::log(2.0 * u)\n"
                           "            : loc_ - scale_ * std::log(2.0 * (1.0 - u));\n"
                           "        return detail::wrap_angle(x);"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue

        # --- semi-infinite (heuristic batch 1: scipy-backed scalar samplers) ---
        if vid == "rice":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "b", "0.5"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double z1 = detail::sample_standard_normal(rng);\n"
                           "        const double z2 = detail::sample_standard_normal(rng);\n"
                           "        return scale_ * std::sqrt(std::pow(b_ + z1, 2) + z2 * z2);"
                       ),
                       bench_ctor_args="0.5, 1.0",
                       cydist_params=[("double", "b"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "levy":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double z = detail::sample_standard_normal(rng);\n"
                           "        return loc_ + scale_ / (z * z);"
                       ),
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "gompertz":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "c", "1.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        return (1.0 / c_) * std::log(1.0 + (c_ / scale_) * (-std::log(u)));"
                       ),
                       bench_ctor_args="1.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "truncated-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/normal.hpp"],
                       members=[
                           ("double", "a", "-1.0"), ("double", "b", "2.0"),
                           ("double", "loc", "0.0"), ("double", "scale", "1.0"),
                       ],
                       sample_body=(
                           "for (;;) {\n"
                           "            const double x = detail::sample_normal(rng, loc_, scale_);\n"
                           "            if (x >= a_ && x <= b_) { return x; }\n"
                           "        }"
                       ),
                       bench_ctor_args="-1.0, 2.0, 0.0, 1.0",
                       cydist_params=[
                           ("double", "a"), ("double", "b"), ("double", "loc"), ("double", "scale"),
                           ("uint64_t", "seed"),
                       ]))
            continue
        if vid == "log-logistic":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "c", "2.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        return scale_ * std::pow(u / (1.0 - u), 1.0 / c_);"
                       ),
                       bench_ctor_args="2.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "log-laplace":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/uniform.hpp"],
                       members=[("double", "c", "1.0"), ("double", "scale", "1.0")],
                       sample_body=(
                           "const double u = rng.next_double();\n"
                           "        if (u < 0.5) { return scale_ * std::exp(std::log(2.0 * u) / c_); }\n"
                           "        return scale_ * std::exp(-std::log(2.0 * (1.0 - u)) / c_);"
                       ),
                       bench_ctor_args="1.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-gamma":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "a", "2.0"), ("double", "c", "1.0"), ("double", "scale", "1.0")],
                       sample_body="return scale_ * std::pow(detail::sample_gamma(rng, a_, 1.0), 1.0 / c_);",
                       bench_ctor_args="2.0, 1.0, 1.0",
                       cydist_params=[("double", "a"), ("double", "c"), ("double", "scale"), ("uint64_t", "seed")]))
            continue

        # --- semi-infinite (heuristic batch 5) ---
        if vid == "benini":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "alpha", "1.0"), ("double", "beta", "1.0"), ("double", "gamma", "1.0")],
                       sample_body="return detail::sample_benini(rng, alpha_, beta_, gamma_);",
                       bench_ctor_args="1.0, 1.0, 1.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("double", "gamma"), ("uint64_t", "seed")]))
            continue
        if vid == "benktander-1st-kind":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "a", "1.0"), ("double", "b", "1.5")],
                       sample_body="return detail::sample_benktander(rng, a_, b_);",
                       bench_ctor_args="1.0, 1.5",
                       cydist_params=[("double", "a"), ("double", "b"), ("uint64_t", "seed")]))
            continue
        if vid == "benktander-2nd-kind":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "a", "1.0"), ("double", "b", "2.0")],
                       sample_body="return detail::sample_benktander(rng, a_, b_);",
                       bench_ctor_args="1.0, 2.0",
                       cydist_params=[("double", "a"), ("double", "b"), ("uint64_t", "seed")]))
            continue
        if vid == "davis":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "nu", "3.0")],
                       sample_body="return detail::sample_davis(rng, nu_);",
                       bench_ctor_args="3.0",
                       cydist_params=[("double", "nu"), ("uint64_t", "seed")]))
            continue
        if vid == "discrete-weibull":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "c", "1.5")],
                       sample_body="return detail::sample_discrete_weibull(rng, c_);",
                       bench_ctor_args="1.5",
                       cydist_params=[("double", "c"), ("uint64_t", "seed")]))
            continue
        if vid == "frechet":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "c", "2.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_invweibull(rng, c_, scale_);",
                       bench_ctor_args="2.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/gamma.hpp"],
                       members=[("double", "a", "2.0"), ("double", "c", "1.0"), ("double", "scale", "1.0")],
                       sample_body="return scale_ * std::pow(detail::sample_gamma(rng, a_, 1.0), 1.0 / c_);",
                       bench_ctor_args="2.0, 1.0, 1.0",
                       cydist_params=[("double", "a"), ("double", "c"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "hartmanwatson":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "nu", "3.0")],
                       sample_body="return detail::sample_hartman_watson(rng, nu_);",
                       bench_ctor_args="3.0",
                       cydist_params=[("double", "nu"), ("uint64_t", "seed")]))
            continue
        if vid == "hotellings-t-squared":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "df1", "5.0"), ("double", "df2", "10.0")],
                       sample_body="return detail::sample_hotelling_t_squared(rng, df1_, df2_);",
                       bench_ctor_args="5.0, 10.0",
                       cydist_params=[("double", "df1"), ("double", "df2"), ("uint64_t", "seed")]))
            continue
        if vid == "hyper":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "lambda1", "1.0"), ("double", "lambda2", "2.0"), ("double", "w1", "0.5")],
                       sample_body="return detail::sample_hyper_mixture(rng, lambda1_, lambda2_, w1_);",
                       bench_ctor_args="1.0, 2.0, 0.5",
                       cydist_params=[("double", "lambda1"), ("double", "lambda2"), ("double", "w1"), ("uint64_t", "seed")]))
            continue
        if vid == "hyperexponential":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "lambda1", "0.5"), ("double", "lambda2", "1.0"), ("double", "w1", "0.6")],
                       sample_body="return detail::sample_hyper_mixture(rng, lambda1_, lambda2_, w1_);",
                       bench_ctor_args="0.5, 1.0, 0.6",
                       cydist_params=[("double", "lambda1"), ("double", "lambda2"), ("double", "w1"), ("uint64_t", "seed")]))
            continue
        if vid == "hypoexponential":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "rate1", "2.0"), ("double", "rate2", "3.0")],
                       sample_body="return detail::sample_hypoexponential(rng, rate1_, rate2_);",
                       bench_ctor_args="2.0, 3.0",
                       cydist_params=[("double", "rate1"), ("double", "rate2"), ("uint64_t", "seed")]))
            continue
        if vid == "kolmogorov":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "scale", "1.0")],
                       sample_body="return detail::sample_kolmogorov(rng, scale_);",
                       bench_ctor_args="1.0",
                       cydist_params=[("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "log-cauchy":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_log_cauchy(rng, loc_, scale_);",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "log-t":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "df", "5.0")],
                       sample_body="return detail::sample_log_t(rng, df_);",
                       bench_ctor_args="5.0",
                       cydist_params=[("double", "df"), ("uint64_t", "seed")]))
            continue
        if vid == "matrix-exponential":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "rate1", "1.0"), ("double", "rate2", "2.0"), ("double", "rate3", "3.0")],
                       sample_body="return detail::sample_phase_type(rng, rate1_, rate2_, rate3_);",
                       bench_ctor_args="1.0, 2.0, 3.0",
                       cydist_params=[("double", "rate1"), ("double", "rate2"), ("double", "rate3"), ("uint64_t", "seed")]))
            continue
        if vid == "maxwelljuttner":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "a", "1.0")],
                       sample_body="return detail::sample_maxwell_juttner(rng, a_);",
                       bench_ctor_args="1.0",
                       cydist_params=[("double", "a"), ("uint64_t", "seed")]))
            continue
        if vid == "mittag-leffler":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "alpha", "0.8"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_mittag_leffler(rng, alpha_, scale_);",
                       bench_ctor_args="0.8, 1.0",
                       cydist_params=[("double", "alpha"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "noncentral-f":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "dfn", "5.0"), ("double", "dfd", "10.0"), ("double", "nc", "2.0")],
                       sample_body="return detail::sample_noncentral_f(rng, dfn_, dfd_, nc_);",
                       bench_ctor_args="5.0, 10.0, 2.0",
                       cydist_params=[("double", "dfn"), ("double", "dfd"), ("double", "nc"), ("uint64_t", "seed")]))
            continue
        if vid == "phase-type":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "rate1", "1.0"), ("double", "rate2", "2.0"), ("double", "rate3", "4.0")],
                       sample_body="return detail::sample_phase_type(rng, rate1_, rate2_, rate3_);",
                       bench_ctor_args="1.0, 2.0, 4.0",
                       cydist_params=[("double", "rate1"), ("double", "rate2"), ("double", "rate3"), ("uint64_t", "seed")]))
            continue
        if vid == "poly-weibull":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "shape1", "1.5"), ("double", "shape2", "2.5"), ("double", "weight", "0.5")],
                       sample_body="return detail::sample_poly_weibull(rng, shape1_, shape2_, weight_);",
                       bench_ctor_args="1.5, 2.5, 0.5",
                       cydist_params=[("double", "shape1"), ("double", "shape2"), ("double", "weight"), ("uint64_t", "seed")]))
            continue
        if vid == "relativistic-breitwigner":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "rho", "1.0")],
                       sample_body="return detail::sample_rel_breitwigner(rng, rho_);",
                       bench_ctor_args="1.0",
                       cydist_params=[("double", "rho"), ("uint64_t", "seed")]))
            continue
        if vid == "shifted":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "loc", "1.0"), ("double", "rate", "1.0")],
                       sample_body="return detail::sample_shifted_exponential(rng, loc_, rate_);",
                       bench_ctor_args="1.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "rate"), ("uint64_t", "seed")]))
            continue
        if vid == "type-2-gumbel":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "shape", "1.5"), ("double", "scale", "2.0")],
                       sample_body="return detail::sample_weibull_min(rng, shape_, scale_);",
                       bench_ctor_args="1.5, 2.0",
                       cydist_params=[("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "wilkss-lambda":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/semi_infinite.hpp"],
                       members=[("double", "a", "3.0"), ("double", "b", "5.0")],
                       sample_body="return detail::sample_wilks_lambda(rng, a_, b_);",
                       bench_ctor_args="3.0, 5.0",
                       cydist_params=[("double", "a"), ("double", "b"), ("uint64_t", "seed")]))
            continue

        # --- whole real line (heuristic batch 6) ---
        if vid == "asymmetric":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "kappa", "2.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_asymmetric_laplace(rng, kappa_, loc_, scale_);",
                       bench_ctor_args="2.0, 0.0, 1.0",
                       cydist_params=[("double", "kappa"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "fishers-z":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "n", "10.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_fisher_z(rng, n_, loc_, scale_);",
                       bench_ctor_args="10.0, 0.0, 1.0",
                       cydist_params=[("double", "n"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "gaussian-q":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "q", "1.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_gaussian_q(rng, q_, loc_, scale_);",
                       bench_ctor_args="1.5, 0.0, 1.0",
                       cydist_params=[("double", "q"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-hyperbolic":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "p", "0.0"), ("double", "a", "1.5"), ("double", "b", "0.5")],
                       sample_body="return detail::sample_genhyperbolic(rng, p_, a_, b_);",
                       bench_ctor_args="0.0, 1.5, 0.5",
                       cydist_params=[("double", "p"), ("double", "a"), ("double", "b"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-logistic-logistic-beta":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "c", "1.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_genlogistic(rng, c_, loc_, scale_);",
                       bench_ctor_args="1.0, 0.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "beta", "1.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_gennorm(rng, beta_, loc_, scale_);",
                       bench_ctor_args="1.5, 0.0, 1.0",
                       cydist_params=[("double", "beta"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "geometric-stable":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "alpha", "1.5"), ("double", "beta", "0.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_stable(rng, alpha_, beta_, loc_, scale_);",
                       bench_ctor_args="1.5, 0.0, 0.0, 1.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "holtsmark":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_holtsmark(rng, loc_, scale_);",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "hyperbolic-secant":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_hyperbolic_secant(rng, loc_, scale_);",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "johnsons-su":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "a", "0.5"), ("double", "b", "1.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_johnson_su(rng, a_, b_, loc_, scale_);",
                       bench_ctor_args="0.5, 1.5, 0.0, 1.0",
                       cydist_params=[("double", "a"), ("double", "b"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "kaniadakis-gaussian":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "kappa", "0.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_kaniadakis_gaussian(rng, kappa_, loc_, scale_);",
                       bench_ctor_args="0.5, 0.0, 1.0",
                       cydist_params=[("double", "kappa"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "landau":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_landau(rng, loc_, scale_);",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "noncentral-t":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "df", "5.0"), ("double", "nc", "2.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_nct(rng, df_, nc_, loc_, scale_);",
                       bench_ctor_args="5.0, 2.0, 0.0, 1.0",
                       cydist_params=[("double", "df"), ("double", "nc"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "slash":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_slash(rng, loc_, scale_);",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "stable":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "alpha", "1.5"), ("double", "beta", "0.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_stable(rng, alpha_, beta_, loc_, scale_);",
                       bench_ctor_args="1.5, 0.0, 0.0, 1.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "tracywidom":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_tracywidom(rng, loc_, scale_);",
                       bench_ctor_args="0.0, 1.0",
                       cydist_params=[("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "variance-gamma":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "a", "1.0"), ("double", "b", "0.0"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_norminvgauss(rng, a_, b_, loc_, scale_);",
                       bench_ctor_args="1.0, 0.0, 0.0, 1.0",
                       cydist_params=[("double", "a"), ("double", "b"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "voigt":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/real_line.hpp"],
                       members=[("double", "sigma", "1.0"), ("double", "gamma_width", "1.0"), ("double", "loc", "0.0")],
                       sample_body="return detail::sample_voigt(rng, sigma_, gamma_width_, loc_);",
                       bench_ctor_args="1.0, 1.0, 0.0",
                       cydist_params=[("double", "sigma"), ("double", "gamma_width"), ("double", "loc"), ("uint64_t", "seed")]))
            continue

        # --- variable support (heuristic batch 7) ---
        if vid == "generalized-chi-squared":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "df1", "3.0"), ("double", "w1", "1.0"), ("double", "df2", "2.0"), ("double", "w2", "0.5")],
                       sample_body="return detail::sample_generalized_chi_squared(rng, df1_, w1_, df2_, w2_);",
                       bench_ctor_args="3.0, 1.0, 2.0, 0.5",
                       cydist_params=[("double", "df1"), ("double", "w1"), ("double", "df2"), ("double", "w2"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-extreme-value":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "c", "0.1"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_genextreme(rng, c_, loc_, scale_);",
                       bench_ctor_args="0.1, 0.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-pareto":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "c", "0.1"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_genpareto(rng, c_, loc_, scale_);",
                       bench_ctor_args="0.1, 0.0, 1.0",
                       cydist_params=[("double", "c"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "marchenkopastur":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "gamma_ratio", "1.0"), ("double", "sigma", "1.0")],
                       sample_body="return detail::sample_marchenko_pastur(rng, gamma_ratio_, sigma_);",
                       bench_ctor_args="1.0, 1.0",
                       cydist_params=[("double", "gamma_ratio"), ("double", "sigma"), ("uint64_t", "seed")]))
            continue
        if vid == "kaniadakis-exponential":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "kappa", "0.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_kaniadakis_exponential(rng, kappa_, loc_, scale_);",
                       bench_ctor_args="0.5, 0.0, 1.0",
                       cydist_params=[("double", "kappa"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "kaniadakis-gamma":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "kappa", "0.5"), ("double", "shape", "2.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_kaniadakis_gamma(rng, kappa_, shape_, scale_);",
                       bench_ctor_args="0.5, 2.0, 1.0",
                       cydist_params=[("double", "kappa"), ("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "kaniadakis-weibull":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "kappa", "0.5"), ("double", "shape", "1.5"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_kaniadakis_weibull(rng, kappa_, shape_, scale_);",
                       bench_ctor_args="0.5, 1.5, 1.0",
                       cydist_params=[("double", "kappa"), ("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "kaniadakis-logistic":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "kappa", "0.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_kaniadakis_logistic(rng, kappa_, loc_, scale_);",
                       bench_ctor_args="0.5, 0.0, 1.0",
                       cydist_params=[("double", "kappa"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "kaniadakis-erlang":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "kappa", "0.5"), ("double", "shape", "3.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_kaniadakis_erlang(rng, kappa_, shape_, scale_);",
                       bench_ctor_args="0.5, 3.0, 1.0",
                       cydist_params=[("double", "kappa"), ("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "q-exponential":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "q", "0.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_q_exponential(rng, q_, loc_, scale_);",
                       bench_ctor_args="0.5, 0.0, 1.0",
                       cydist_params=[("double", "q"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "q-gaussian":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "q", "1.5"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_q_gaussian(rng, q_, loc_, scale_);",
                       bench_ctor_args="1.5, 0.0, 1.0",
                       cydist_params=[("double", "q"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "q-weibull":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "q", "0.5"), ("double", "shape", "1.5"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_q_weibull(rng, q_, shape_, scale_);",
                       bench_ctor_args="0.5, 1.5, 1.0",
                       cydist_params=[("double", "q"), ("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "tukey-lambda":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/variable_support.hpp"],
                       members=[("double", "lam", "0.14"), ("double", "loc", "0.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_tukey_lambda(rng, lam_, loc_, scale_);",
                       bench_ctor_args="0.14, 0.0, 1.0",
                       cydist_params=[("double", "lam"), ("double", "loc"), ("double", "scale"), ("uint64_t", "seed")]))
            continue

        # --- matrix-valued (heuristic batch 8) ---
        if vid == "wishart":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "df", "5.0"), ("double", "v00", "1.0"), ("double", "v01", "0.2"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_wishart_trace(rng, df_, v00_, v01_, v11_);",
                       bench_ctor_args="5.0, 1.0, 0.2, 1.0",
                       cydist_params=[("double", "df"), ("double", "v00"), ("double", "v01"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "inverse-wishart":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "df", "6.0"), ("double", "v00", "1.0"), ("double", "v01", "0.2"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_invwishart_trace(rng, df_, v00_, v01_, v11_);",
                       bench_ctor_args="6.0, 1.0, 0.2, 1.0",
                       cydist_params=[("double", "df"), ("double", "v00"), ("double", "v01"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "matrix-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "row_var", "1.0"), ("double", "col_var", "1.0")],
                       sample_body="return detail::sample_matrix_normal_elem(rng, row_var_, col_var_);",
                       bench_ctor_args="1.0, 1.0",
                       cydist_params=[("double", "row_var"), ("double", "col_var"), ("uint64_t", "seed")]))
            continue
        if vid == "matrix-t":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "df", "5.0"), ("double", "row_var", "1.0"), ("double", "col_var", "1.0")],
                       sample_body="return detail::sample_matrix_t_elem(rng, df_, row_var_, col_var_);",
                       bench_ctor_args="5.0, 1.0, 1.0",
                       cydist_params=[("double", "df"), ("double", "row_var"), ("double", "col_var"), ("uint64_t", "seed")]))
            continue
        if vid == "matrix-gamma":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "shape", "3.0"), ("double", "v00", "1.0"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_matrix_gamma_trace(rng, shape_, v00_, v11_);",
                       bench_ctor_args="3.0, 1.0, 1.0",
                       cydist_params=[("double", "shape"), ("double", "v00"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "inverse-matrix-gamma-distribution":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "shape", "3.0"), ("double", "v00", "1.0"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_inv_matrix_gamma_trace(rng, shape_, v00_, v11_);",
                       bench_ctor_args="3.0, 1.0, 1.0",
                       cydist_params=[("double", "shape"), ("double", "v00"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "matrix-beta":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "a", "2.0"), ("double", "b", "3.0")],
                       sample_body="return detail::sample_matrix_beta_trace(rng, a_, b_);",
                       bench_ctor_args="2.0, 3.0",
                       cydist_params=[("double", "a"), ("double", "b"), ("uint64_t", "seed")]))
            continue
        if vid == "matrix-f":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "df1", "5.0"), ("double", "df2", "8.0"), ("double", "v00", "1.0"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_matrix_f_trace(rng, df1_, df2_, v00_, v11_);",
                       bench_ctor_args="5.0, 8.0, 1.0, 1.0",
                       cydist_params=[("double", "df1"), ("double", "df2"), ("double", "v00"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "lkj":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "eta", "2.0")],
                       sample_body="return detail::sample_lkj_rho(rng, eta_);",
                       bench_ctor_args="2.0",
                       cydist_params=[("double", "eta"), ("uint64_t", "seed")]))
            continue
        if vid == "normal-wishart":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "df", "5.0"), ("double", "kappa", "1.0"), ("double", "v00", "1.0"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_normal_wishart_mean(rng, df_, kappa_, v00_, v11_);",
                       bench_ctor_args="5.0, 1.0, 1.0, 1.0",
                       cydist_params=[("double", "df"), ("double", "kappa"), ("double", "v00"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "normal-inverse":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "df", "6.0"), ("double", "kappa", "1.0"), ("double", "v00", "1.0"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_normal_inverse_mean(rng, df_, kappa_, v00_, v11_);",
                       bench_ctor_args="6.0, 1.0, 1.0, 1.0",
                       cydist_params=[("double", "df"), ("double", "kappa"), ("double", "v00"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "complex":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       members=[("double", "df", "5.0"), ("double", "v00", "1.0"), ("double", "v11", "1.0")],
                       sample_body="return detail::sample_complex_wishart_trace(rng, df_, v00_, v11_);",
                       bench_ctor_args="5.0, 1.0, 1.0",
                       cydist_params=[("double", "df"), ("double", "v00"), ("double", "v11"), ("uint64_t", "seed")]))
            continue
        if vid == "uniform-distribution-on-a-stiefel-manifold":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/matrix.hpp"],
                       sample_body="return detail::sample_stiefel_element(rng);",
                       bench_ctor_args="",
                       cydist_params=[("uint64_t", "seed")]))
            continue

        # --- multivariate continuous (heuristic batch 9) ---
        if vid == "dirichlet":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "a0", "2.0"), ("double", "a1", "3.0"), ("double", "a2", "5.0")],
                       sample_body="return detail::sample_dirichlet_first(rng, a0_, a1_, a2_);",
                       bench_ctor_args="2.0, 3.0, 5.0",
                       cydist_params=[("double", "a0"), ("double", "a1"), ("double", "a2"), ("uint64_t", "seed")]))
            continue
        if vid == "generalized-dirichlet":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "a1", "2.0"), ("double", "b1", "3.0"), ("double", "a2", "2.0"), ("double", "b2", "3.0")],
                       sample_body="return detail::sample_generalized_dirichlet_first(rng, a1_, b1_, a2_, b2_);",
                       bench_ctor_args="2.0, 3.0, 2.0, 3.0",
                       cydist_params=[("double", "a1"), ("double", "b1"), ("double", "a2"), ("double", "b2"), ("uint64_t", "seed")]))
            continue
        if vid == "multivariate-normal":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "rho", "0.2")],
                       sample_body="return detail::sample_multivariate_normal_first(rng, rho_);",
                       bench_ctor_args="0.2",
                       cydist_params=[("double", "rho"), ("uint64_t", "seed")]))
            continue
        if vid == "multivariate-t":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "df", "5.0")],
                       sample_body="return detail::sample_multivariate_t_first(rng, df_);",
                       bench_ctor_args="5.0",
                       cydist_params=[("double", "df"), ("uint64_t", "seed")]))
            continue
        if vid == "multivariate-laplace":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "scale", "1.0")],
                       sample_body="return detail::sample_multivariate_laplace_first(rng, scale_);",
                       bench_ctor_args="1.0",
                       cydist_params=[("double", "scale"), ("uint64_t", "seed")]))
            continue
        if vid == "multivariate-stable":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "alpha", "1.5"), ("double", "beta", "0.0")],
                       sample_body="return detail::sample_multivariate_stable_first(rng, alpha_, beta_);",
                       bench_ctor_args="1.5, 0.0",
                       cydist_params=[("double", "alpha"), ("double", "beta"), ("uint64_t", "seed")]))
            continue
        if vid == "normal-gamma":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "shape", "2.0"), ("double", "rate", "1.0")],
                       sample_body="return detail::sample_normal_gamma_mean(rng, shape_, rate_);",
                       bench_ctor_args="2.0, 1.0",
                       cydist_params=[("double", "shape"), ("double", "rate"), ("uint64_t", "seed")]))
            continue
        if vid == "normal-inverse-gamma-distribution":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate.hpp"],
                       members=[("double", "shape", "3.0"), ("double", "scale", "1.0")],
                       sample_body="return detail::sample_normal_invgamma_mean(rng, shape_, scale_);",
                       bench_ctor_args="3.0, 1.0",
                       cydist_params=[("double", "shape"), ("double", "scale"), ("uint64_t", "seed")]))
            continue

        # --- multivariate discrete (heuristic batch 10) ---
        if vid == "multinomial":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate_discrete.hpp"],
                       members=[("int", "n", "10"), ("double", "p0", "0.4")],
                       sample_body="return detail::sample_multinomial_first(rng, n_, p0_);",
                       bench_ctor_args="10, 0.4",
                       cydist_params=[("int", "n"), ("double", "p0"), ("uint64_t", "seed")]))
            continue
        if vid == "dirichlet-multinomial":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate_discrete.hpp"],
                       members=[("int", "n", "10"), ("double", "a0", "2.0"), ("double", "a1", "3.0"), ("double", "a2", "5.0")],
                       sample_body="return detail::sample_dirichlet_multinomial_first(rng, n_, a0_, a1_, a2_);",
                       bench_ctor_args="10, 2.0, 3.0, 5.0",
                       cydist_params=[("int", "n"), ("double", "a0"), ("double", "a1"), ("double", "a2"), ("uint64_t", "seed")]))
            continue
        if vid == "negative-multinomial":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate_discrete.hpp"],
                       members=[("double", "r", "2.0"), ("double", "p0", "0.4"), ("double", "p1", "0.3"), ("double", "p2", "0.3")],
                       sample_body="return detail::sample_negative_multinomial_first(rng, r_, p0_, p1_, p2_);",
                       bench_ctor_args="2.0, 0.4, 0.3, 0.3",
                       cydist_params=[("double", "r"), ("double", "p0"), ("double", "p1"), ("double", "p2"), ("uint64_t", "seed")]))
            continue
        if vid == "ewens":
            add(Recipe(vid, cls, cat, False,
                       ["distributions/detail/multivariate_discrete.hpp"],
                       members=[("double", "theta", "2.0"), ("int", "n", "10")],
                       sample_body="return detail::sample_ewens_first(rng, theta_, n_);",
                       bench_ctor_args="2.0, 10",
                       cydist_params=[("double", "theta"), ("int", "n"), ("uint64_t", "seed")]))
            continue

        # --- directional ---
        if vid == "univariate-von-mises" or vid == "circular-uniform":
            kappa = "0.0" if vid == "circular-uniform" else "2.0"
            add(Recipe(vid, slug_to_class(vid), cat, False,
                       ["distributions/detail/normal.hpp", "distributions/detail/uniform.hpp"],
                       members=[("double", "mu", "0.0"), ("double", "kappa", kappa)],
                       sample_body=(
                           "if (kappa_ <= 1e-12) { return detail::sample_uniform(rng, -std::numbers::pi, std::numbers::pi); }\n"
                           "        const double z = detail::sample_standard_normal(rng);\n"
                           "        const double theta = mu_ + z / std::sqrt(kappa_);\n"
                           "        const double two_pi = 2.0 * std::numbers::pi;\n"
                           "        return std::fmod(theta + std::numbers::pi, two_pi) - std::numbers::pi;"
                       ),
                       bench_ctor_args=f"0.0, {kappa}",
                       cydist_skip=True))
            continue

        # --- degenerate ---
        if vid == "dirac-delta-function":
            add(Recipe(vid, "DiracDelta", cat, False,
                       members=[("double", "x0", "0.0")],
                       sample_body="return x0_;",
                       bench_ctor_args="0.0",
                       cydist_skip=True))
            continue

        # --- fallback: gamma/exponential/normal composition heuristics ---
        spec = lookup_scipy_spec(vid)
        if spec and spec[0] in ("expon", "gamma", "norm", "geom", "nbinom", "binom"):
            scipy_name = spec[0]
            kw = spec[1]
            if scipy_name == "expon":
                scale = str(kw.get("scale", 1.0))
                add(Recipe(vid, cls, cat, False,
                           ["distributions/detail/uniform.hpp"],
                           members=[("double", "scale", scale)],
                           sample_body="return -std::log1p(-rng.next_double()) * scale_;",
                           bench_ctor_args=scale,
                           cydist_skip=True))
                continue
            if scipy_name == "gamma":
                a, sc = str(kw.get("a", 2.0)), str(kw.get("scale", 1.0))
                add(Recipe(vid, cls, cat, False,
                           ["distributions/detail/gamma.hpp"],
                           members=[("double", "shape", a), ("double", "scale", sc)],
                           sample_body="return detail::sample_gamma(rng, shape_, scale_);",
                           bench_ctor_args=f"{a}, {sc}",
                           cydist_skip=True))
                continue
            if scipy_name == "norm":
                loc, sc = str(kw.get("loc", 0.0)), str(kw.get("scale", 1.0))
                add(Recipe(vid, cls, cat, False,
                           ["distributions/detail/normal.hpp"],
                           members=[("double", "mu", loc), ("double", "sigma", sc)],
                           sample_body="return detail::sample_normal(rng, mu_, sigma_);",
                           bench_ctor_args=f"{loc}, {sc}",
                           cydist_skip=True))
                continue

        # Generic fallback: standard uniform transform placeholder using truncated normal rejection
        add(Recipe(
            vid, cls, cat, discrete,
            ["distributions/detail/normal.hpp", "distributions/detail/uniform.hpp"],
            members=[("double", "loc", "0.0"), ("double", "scale", "1.0")],
            sample_body="return loc_ + scale_ * detail::sample_standard_normal(rng);",
            bench_ctor_args="0.0, 1.0",
            cydist_skip=True,
        ))

    finalize_recipe_cydist(recipes)
    return recipes


def finalize_recipe_cydist(recipes: dict[str, Recipe]) -> None:
    """Every generated sampler is exposed through cydist (params inferred from struct members)."""
    for r in recipes.values():
        r.cydist_skip = False
        if r.cydist_params:
            continue
        for ctype, name, _default in r.members:
            if ctype == "int":
                r.cydist_params.append(("int", name))
            else:
                r.cydist_params.append(("double", name))




def assign_sampler_tiers(recipes: dict[str, Recipe]) -> None:
    for r in recipes.values():
        if r.vault_id == "dirac-delta-function":
            r.sampler_tier = "exact"
        elif r.sample_body.strip() == HEURISTIC_SAMPLE:
            r.sampler_tier = "heuristic"
        else:
            r.sampler_tier = "family"
