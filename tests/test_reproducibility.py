"""Tier A vs Tier B statistical equivalence for hand-written fast paths.

Tier A: ``sample_batch`` in chunks below ``kFastThreshold`` (serial PCG loop).
Tier B: one large ``sample_batch`` (fast path when implemented).

Outputs are not bit-identical; we require compatible moments / KS / chi-square.
"""

from __future__ import annotations

import numpy as np
import pytest

import cydist
from statistical_compare import check_continuous, check_discrete

# Must match include/distributions/detail/counter_rng.hpp
K_FAST_THRESHOLD = 4096
TIER_A_CHUNK = 2048
N_SAMPLES = 100_000
ALPHA = 0.01

# (cydist func, kwargs, discrete)
TIER_B_CASES: list[tuple[str, dict, bool]] = [
    ("bernoulli_sample_batch", {"p": 0.35}, True),
    ("discrete_uniform_sample_batch", {"low": 1, "high": 7}, True),
    ("exponential_sample_batch", {"rate": 1.5}, False),
    ("normal_sample_batch", {"mu": 1.0, "sigma": 0.5}, False),
    ("binomial_sample_batch", {"trials": 20, "p": 0.4}, True),
    ("beta_binomial_sample_batch", {"trials": 20, "alpha": 2.0, "beta": 5.0}, True),
    ("negative_binomial_sample_batch", {"r": 5, "p": 0.4}, True),
    ("poisson_binomial_sample_batch", {
        "probs": np.array([0.1, 0.2, 0.3, 0.4], dtype=np.float64),
    }, True),
    ("categorical_sample_batch", {
        "probs": np.array([0.5, 0.3, 0.2], dtype=np.float64),
    }, True),
    ("zipf_sample_batch", {"N": 10, "s": 2.0}, True),
    ("zipf_mandelbrot_sample_batch", {"N": 10, "q": 1.0, "s": 2.0}, True),
    ("skellam_sample_batch", {"mu1": 3.0, "mu2": 1.0}, True),
]


def _sample_tier_a(fn, out: np.ndarray, kwargs: dict, seed: int) -> None:
    pos = 0
    chunk_id = 0
    while pos < len(out):
        n = min(TIER_A_CHUNK, len(out) - pos)
        chunk = np.empty(n, dtype=out.dtype)
        fn(chunk, seed=seed + chunk_id, **kwargs)
        out[pos : pos + n] = chunk
        pos += n
        chunk_id += 1
    assert TIER_A_CHUNK < K_FAST_THRESHOLD


def _sample_tier_b(fn, out: np.ndarray, kwargs: dict, seed: int) -> None:
    assert len(out) >= K_FAST_THRESHOLD
    fn(out, seed=seed, **kwargs)


@pytest.mark.parametrize(
    "func_name, kwargs, discrete",
    TIER_B_CASES,
    ids=[c[0].removesuffix("_sample_batch") for c in TIER_B_CASES],
)
def test_tier_a_vs_tier_b(func_name: str, kwargs: dict, discrete: bool) -> None:
    fn = getattr(cydist, func_name)
    dtype = np.int32 if discrete else np.float64
    tier_a = np.empty(N_SAMPLES, dtype=dtype)
    tier_b = np.empty(N_SAMPLES, dtype=dtype)
    _sample_tier_a(fn, tier_a, kwargs, seed=42)
    _sample_tier_b(fn, tier_b, kwargs, seed=99)
    if discrete:
        ok, msg = check_discrete(tier_a, tier_b, ALPHA)
    else:
        ok, msg = check_continuous(tier_a, tier_b, ALPHA)
    assert ok, msg
