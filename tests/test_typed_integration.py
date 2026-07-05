"""Phase 3 typed API integration sign-off (batch 8).

Smoke all 13 hand-written cydist entry points with documented dtypes and verify
int32 Tier A vs Tier B statistical equivalence where fast paths exist.
"""

from __future__ import annotations

import numpy as np
import pytest

import cydist
from statistical_compare import check_continuous, check_discrete

K_FAST_THRESHOLD = 4096
TIER_A_CHUNK = 2048
N_SAMPLES = 100_000
ALPHA = 0.01
LARGE_N = 8192

# (func, kwargs, dtype)
HAND_WRITTEN_CASES: list[tuple[str, dict, type]] = [
    ("bernoulli_sample_batch", {"p": 0.35}, np.int32),
    ("discrete_uniform_sample_batch", {"low": 1, "high": 7}, np.int32),
    ("binomial_sample_batch", {"trials": 20, "p": 0.4}, np.int32),
    ("categorical_sample_batch", {
        "probs": np.array([0.5, 0.3, 0.2], dtype=np.float64),
    }, np.int32),
    ("beta_binomial_sample_batch", {"trials": 20, "alpha": 2.0, "beta": 5.0}, np.int32),
    ("poisson_binomial_sample_batch", {
        "probs": np.array([0.1, 0.2, 0.3, 0.4], dtype=np.float64),
    }, np.int32),
    ("zipf_sample_batch", {"N": 10, "s": 2.0}, np.int32),
    ("zipf_mandelbrot_sample_batch", {"N": 10, "q": 1.0, "s": 2.0}, np.int32),
    ("geometric_sample_batch", {"p": 0.25}, np.int32),
    ("negative_binomial_sample_batch", {"r": 5, "p": 0.4}, np.int32),
    ("skellam_sample_batch", {"mu1": 3.0, "mu2": 1.0}, np.int32),
    ("normal_sample_batch", {"mu": 1.0, "sigma": 0.5}, np.float64),
    ("exponential_sample_batch", {"rate": 1.5}, np.float64),
]

INT32_TIER_REPRO: list[tuple[str, dict]] = [
    (name, kwargs)
    for name, kwargs, dtype in HAND_WRITTEN_CASES
    if dtype is np.int32 and name != "geometric_sample_batch"
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


@pytest.mark.parametrize(
    "func_name, kwargs, dtype",
    HAND_WRITTEN_CASES,
    ids=[c[0].removesuffix("_sample_batch") for c in HAND_WRITTEN_CASES],
)
def test_hand_written_large_batch_smoke(func_name: str, kwargs: dict, dtype: type) -> None:
    fn = getattr(cydist, func_name)
    out = np.empty(LARGE_N, dtype=dtype)
    fn(out, seed=42, **kwargs)
    assert out.dtype == dtype
    if dtype is np.int32:
        assert out.size == LARGE_N
    else:
        assert np.all(np.isfinite(out))


@pytest.mark.parametrize("func_name, kwargs", [
    ("normal_sample_batch", {"mu": 0.0, "sigma": 1.0}),
    ("exponential_sample_batch", {"rate": 1.5}),
])
def test_hand_written_float32_large_batch(func_name: str, kwargs: dict) -> None:
    fn = getattr(cydist, func_name)
    out = np.empty(LARGE_N, dtype=np.float32)
    fn(out, seed=42, **kwargs)
    assert np.all(np.isfinite(out))


@pytest.mark.parametrize(
    "func_name, kwargs",
    INT32_TIER_REPRO,
    ids=[c[0].removesuffix("_sample_batch") for c in INT32_TIER_REPRO],
)
def test_int32_tier_a_vs_tier_b(func_name: str, kwargs: dict) -> None:
    fn = getattr(cydist, func_name)
    tier_a = np.empty(N_SAMPLES, dtype=np.int32)
    tier_b = np.empty(N_SAMPLES, dtype=np.int32)
    _sample_tier_a(fn, tier_a, kwargs, seed=42)
    fn(tier_b, seed=99, **kwargs)
    assert len(tier_b) >= K_FAST_THRESHOLD
    ok, msg = check_discrete(tier_a, tier_b, ALPHA)
    assert ok, msg


@pytest.mark.parametrize(
    "func_name, kwargs",
    [
        ("normal_sample_batch", {"mu": 1.0, "sigma": 0.5}),
        ("exponential_sample_batch", {"rate": 1.5}),
    ],
)
def test_float64_tier_a_vs_tier_b(func_name: str, kwargs: dict) -> None:
    fn = getattr(cydist, func_name)
    tier_a = np.empty(N_SAMPLES, dtype=np.float64)
    tier_b = np.empty(N_SAMPLES, dtype=np.float64)
    _sample_tier_a(fn, tier_a, kwargs, seed=42)
    fn(tier_b, seed=99, **kwargs)
    ok, msg = check_continuous(tier_a, tier_b, ALPHA)
    assert ok, msg


@pytest.mark.parametrize(
    "func_name, kwargs",
    [
        ("normal_sample_batch", {"mu": 1.0, "sigma": 0.5}),
        ("exponential_sample_batch", {"rate": 1.5}),
    ],
)
def test_float32_tier_a_vs_tier_b(func_name: str, kwargs: dict) -> None:
    fn = getattr(cydist, func_name)
    tier_a = np.empty(N_SAMPLES, dtype=np.float32)
    tier_b = np.empty(N_SAMPLES, dtype=np.float32)
    _sample_tier_a(fn, tier_a, kwargs, seed=42)
    fn(tier_b, seed=99, **kwargs)
    ok, msg = check_continuous(
        tier_a.astype(np.float64), tier_b.astype(np.float64), ALPHA
    )
    assert ok, msg
