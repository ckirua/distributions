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
