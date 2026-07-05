"""Tier A vs Tier B statistical equivalence for float32 hand-written paths.

Applies to ``normal_sample_batch`` and ``exponential_sample_batch`` with
``dtype=float32`` (``NormalDistribution<float>`` / ``ExponentialDistribution<float>``).
"""

from __future__ import annotations

import numpy as np
import pytest

import cydist
from statistical_compare import check_continuous

K_FAST_THRESHOLD = 4096
TIER_A_CHUNK = 2048
N_SAMPLES = 100_000
ALPHA = 0.01

FLOAT_TIER_B_CASES: list[tuple[str, dict]] = [
    ("normal_sample_batch", {"mu": 1.0, "sigma": 0.5}),
    ("exponential_sample_batch", {"rate": 1.5}),
]


def _sample_tier_a(fn, out: np.ndarray, kwargs: dict, seed: int) -> None:
    pos = 0
    chunk_id = 0
    while pos < len(out):
        n = min(TIER_A_CHUNK, len(out) - pos)
        chunk = np.empty(n, dtype=np.float32)
        fn(chunk, seed=seed + chunk_id, **kwargs)
        out[pos : pos + n] = chunk
        pos += n
        chunk_id += 1
    assert TIER_A_CHUNK < K_FAST_THRESHOLD


def _sample_tier_b(fn, out: np.ndarray, kwargs: dict, seed: int) -> None:
    assert len(out) >= K_FAST_THRESHOLD
    fn(out, seed=seed, **kwargs)


@pytest.mark.parametrize(
    "func_name, kwargs",
    FLOAT_TIER_B_CASES,
    ids=[c[0].removesuffix("_sample_batch") for c in FLOAT_TIER_B_CASES],
)
def test_float_tier_a_vs_tier_b(func_name: str, kwargs: dict) -> None:
    fn = getattr(cydist, func_name)
    tier_a = np.empty(N_SAMPLES, dtype=np.float32)
    tier_b = np.empty(N_SAMPLES, dtype=np.float32)
    _sample_tier_a(fn, tier_a, kwargs, seed=42)
    _sample_tier_b(fn, tier_b, kwargs, seed=99)
    ok, msg = check_continuous(tier_a.astype(np.float64), tier_b.astype(np.float64), ALPHA)
    assert ok, msg
