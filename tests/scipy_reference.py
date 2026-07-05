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


CUSTOM_SCIPY_SAMPLERS: dict[str, Callable[[dict[str, Any], int, np.random.Generator], np.ndarray]] = {
    "categorical": _sample_categorical,
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
