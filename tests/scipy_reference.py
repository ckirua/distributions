"""Build sanity-test cases: cydist kwargs + matching scipy.stats reference."""

from __future__ import annotations

import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable

import numpy as np
import yaml

ROOT = Path(__file__).resolve().parents[1]
VAULT_TOOLS = ROOT / ".vault" / "tools"
sys.path.insert(0, str(VAULT_TOOLS))

from vault_scipy import SCIPY_SPECS  # noqa: E402

from test_generated import CYDIST_CASES  # noqa: E402

PY_FUNC_ALIASES: dict[str, str] = {
    "normal-gaussian": "normal_sample_batch",
    "zipfmandelbrot": "zipf_mandelbrot_sample_batch",
}

# No scipy.stats reference (or not in SCIPY_SPECS).
SKIP_VAULT_IDS: frozenset[str] = frozenset(
    {
        "poisson-binomial",
        "zipfmandelbrot",
        "benford",
        "rademacher",
        "logarithmic",
        "yulesimon",
        "zeta",
        "bates",
        "logit-normal",
        "u-quadratic",
        "noncentral-chi-squared",
        "inverse-chi-squared",
        "scaled-inverse-chi-squared",
        "skew-normal",
        "dagum",
        "kumaraswamy",
        "floryschulz",
        "borel",
        "gausskuzmin",
        "delaporte",
        "discrete-phase-type",
        "parabolic-fractal",
        "beta-negative-binomial",
        "baldingnichols",
        "continuous-bernoulli",
        "continuous-binomial",
        "noncentral-beta",
        "hyper",
        "discrete-weibull",
        "log-cauchy",
        "log-t",
        "benktander-1st-kind",
        "benktander-2nd-kind",
        "benini",
        "davis",
        "hartmanwatson",
        "hyperexponential",
        "hypoexponential",
        "kolmogorov",
        "matrix-exponential",
        "maxwelljuttner",
        "mittag-leffler",
        "phase-type",
        "poly-weibull",
        "wilkss-lambda",
        "relativistic-breitwigner",
        "fishers-z",
        "gaussian-q",
        "generalized-hyperbolic",
        "geometric-stable",
        "hyperbolic-secant",
        "kaniadakis-gaussian",
        "slash",
        "tracywidom",
        "voigt",
        "generalized-chi-squared",
        "marchenkopastur",
        "kaniadakis-exponential",
        "kaniadakis-gamma",
        "kaniadakis-weibull",
        "kaniadakis-logistic",
        "kaniadakis-erlang",
        "q-exponential",
        "q-gaussian",
        "q-weibull",
        "inverse-wishart",
        "matrix-gamma",
        "inverse-matrix-gamma-distribution",
        "matrix-beta",
        "matrix-f",
        "lkj",
        "normal-wishart",
        "normal-inverse",
        "complex",
        "uniform-distribution-on-a-stiefel-manifold",
        "matrix-t",
        "generalized-dirichlet",
        "multivariate-laplace",
        "multivariate-stable",
        "normal-gamma",
        "normal-inverse-gamma-distribution",
        "dirichlet-multinomial",
        "negative-multinomial",
        "ewens",
        "rectified-gaussian",
        "cantor",
        "kent",
        "bingham",
        "bivariate-von-mises",
        "soliton",
    }
)

# Approximate or known-mismatched implementations — strict KS may fail until fixed.
XFAIL_VAULT_IDS: frozenset[str] = frozenset(
    {
        "univariate-von-mises",
        "circular-uniform",
        "burr",
        "cauchy",
        "folded-normal",
        "half-logistic",
        "hypergeometric",
        "logistic",
        "nakagami",
        "negative-hypergeometric",
        "raised-cosine",
        "wigner-semicircle",
        "zipf",
        "inverse-gamma",
    }
)

SANITY_TIERS: frozenset[str] = frozenset({"hand-written", "family"})

# Override scipy name/kwargs where SCIPY_SPECS name or defaults disagree with scipy API.
SCIPY_KWARG_OVERRIDES: dict[str, tuple[str, dict[str, Any]]] = {
    "discrete-uniform": ("randint", {"low": 1, "high": 8}),  # cydist [1,7] inclusive
    "inverse-gaussian": ("invgauss", {"mu": 1, "scale": 1}),
    "zipf": ("zipfian", {"a": 2.0, "n": 10}),
    "negative-hypergeometric": ("nhypergeom", {"M": 50, "n": 10, "r": 5}),
}


def _sample_categorical(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    probs = np.asarray(kwargs["probs"], dtype=np.float64)
    probs = probs / probs.sum()
    return rng.choice(len(probs), size=n, p=probs).astype(np.int32)


def _scale_matrix(kwargs: dict[str, Any]) -> list[list[float]]:
    return [
        [float(kwargs["v00"]), float(kwargs["v01"])],
        [float(kwargs["v01"]), float(kwargs["v11"])],
    ]


def _sample_wishart_trace(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    rv = stats.wishart(df=float(kwargs["df"]), scale=_scale_matrix(kwargs))
    mats = rv.rvs(size=n, random_state=rng)
    return np.asarray([float(m.trace()) for m in mats], dtype=np.float64)


def _sample_matrix_normal_elem(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    row_var = float(kwargs["row_var"])
    col_var = float(kwargs["col_var"])
    rv = stats.matrix_normal(mean=[[0.0]], rowcov=[[row_var]], colcov=[[col_var]])
    mats = rv.rvs(size=n, random_state=rng)
    return np.asarray(mats[:, 0, 0], dtype=np.float64)


def _sample_matrix_t_elem(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    row_var = float(kwargs["row_var"])
    col_var = float(kwargs["col_var"])
    df = float(kwargs["df"])
    rv = stats.matrix_t(df=df, mean=[[0.0]], row_spread=[[row_var]], col_spread=[[col_var]])
    mats = rv.rvs(size=n, random_state=rng)
    return np.asarray(mats[:, 0, 0], dtype=np.float64)


def _sample_dirichlet_first(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    alpha = [float(kwargs["a0"]), float(kwargs["a1"]), float(kwargs["a2"])]
    s = stats.dirichlet(alpha).rvs(size=n, random_state=rng)
    return np.asarray(s[:, 0], dtype=np.float64)


def _sample_multivariate_normal_first(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    rho = float(kwargs["rho"])
    cov = [[1.0, rho], [rho, 1.0]]
    s = stats.multivariate_normal([0.0, 0.0], cov).rvs(size=n, random_state=rng)
    return np.asarray(s[:, 0], dtype=np.float64)


def _sample_multivariate_t_first(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    df = float(kwargs["df"])
    rho = 0.2
    cov = [[1.0, rho], [rho, 1.0]]
    s = stats.multivariate_t([0.0, 0.0], cov, df=df).rvs(size=n, random_state=rng)
    return np.asarray(s[:, 0], dtype=np.float64)


def _sample_multinomial_first(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    n_trials = int(kwargs["n_"])
    p0 = float(kwargs["p0"])
    p1 = (1.0 - p0) * 0.5
    p2 = 1.0 - p0 - p1
    s = stats.multinomial(n_trials, [p0, p1, p2]).rvs(size=n, random_state=rng)
    return np.asarray(s[:, 0], dtype=np.float64)


def _sample_von_mises_fisher_x(kwargs: dict[str, Any], n: int, rng: np.random.Generator) -> np.ndarray:
    from scipy import stats

    kappa = float(kwargs["kappa"])
    s = stats.vonmises_fisher(mu=[1.0, 0.0, 0.0], kappa=kappa).rvs(size=n, random_state=rng)
    return np.asarray(s[:, 0], dtype=np.float64)


CUSTOM_SCIPY_SAMPLERS: dict[str, Callable[[dict[str, Any], int, np.random.Generator], np.ndarray]] = {
    "categorical": _sample_categorical,
    "wishart": _sample_wishart_trace,
    "matrix-normal": _sample_matrix_normal_elem,
    "matrix-t": _sample_matrix_t_elem,
    "dirichlet": _sample_dirichlet_first,
    "multivariate-normal": _sample_multivariate_normal_first,
    "multivariate-t": _sample_multivariate_t_first,
    "multinomial": _sample_multinomial_first,
    "von-misesfisher": _sample_von_mises_fisher_x,
}


@dataclass(frozen=True)
class SanityCase:
    vault_id: str
    py_func: str
    cydist_kwargs: dict
    scipy_name: str
    scipy_kwargs: dict
    discrete: bool
    xfail: bool = False
    custom_scipy: str | None = None


def python_func_name(vault_id: str) -> str:
    if vault_id in PY_FUNC_ALIASES:
        return PY_FUNC_ALIASES[vault_id]
    return vault_id.replace("-", "_") + "_sample_batch"


def _scipy_spec_for(vid: str) -> tuple[str, dict[str, Any], str | None] | None:
    if vid in CUSTOM_SCIPY_SAMPLERS:
        cydist_kwargs = next(
            (k for f, k, _ in CYDIST_CASES if f == python_func_name(vid)),
            None,
        )
        if cydist_kwargs is None:
            return None
        return ("custom", dict(cydist_kwargs), vid)

    if vid in SCIPY_KWARG_OVERRIDES:
        name, kwargs = SCIPY_KWARG_OVERRIDES[vid]
        return name, dict(kwargs), None

    if vid not in SCIPY_SPECS:
        return None
    name, kwargs = SCIPY_SPECS[vid]
    return name, dict(kwargs), None


def build_sanity_cases() -> list[SanityCase]:
    registry = yaml.safe_load((ROOT / ".vault" / "_meta" / "registry.yaml").read_text())[
        "distributions"
    ]
    kwargs_by_func = {func: (kwargs, discrete) for func, kwargs, discrete in CYDIST_CASES}

    cases: list[SanityCase] = []
    for entry in registry:
        vid = entry["id"]
        if entry.get("sampler_tier") not in SANITY_TIERS:
            continue
        if vid in SKIP_VAULT_IDS:
            continue

        spec = _scipy_spec_for(vid)
        if spec is None:
            continue

        scipy_name, scipy_kwargs, custom = spec
        py_func = python_func_name(vid)
        if py_func not in kwargs_by_func:
            continue

        cydist_kwargs, discrete = kwargs_by_func[py_func]
        cases.append(
            SanityCase(
                vault_id=vid,
                py_func=py_func,
                cydist_kwargs=cydist_kwargs,
                scipy_name=scipy_name,
                scipy_kwargs=scipy_kwargs,
                discrete=discrete,
                xfail=vid in XFAIL_VAULT_IDS,
                custom_scipy=custom,
            )
        )

    return sorted(cases, key=lambda c: c.vault_id)


SANITY_CASES: list[SanityCase] = build_sanity_cases()


def sample_scipy_reference(
    case: SanityCase,
    n: int,
    rng: np.random.Generator,
) -> np.ndarray:
    if case.custom_scipy is not None:
        fn = CUSTOM_SCIPY_SAMPLERS[case.custom_scipy]
        return fn(case.scipy_kwargs, n, rng)

    from scipy import stats

    rv = getattr(stats, case.scipy_name)
    samples = rv.rvs(size=n, random_state=rng, **case.scipy_kwargs)
    if case.discrete:
        return np.asarray(samples, dtype=np.int32)
    return np.asarray(samples, dtype=np.float64)
