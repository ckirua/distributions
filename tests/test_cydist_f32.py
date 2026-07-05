"""cydist float32 batch paths (API batch 4 — Cython FusedType)."""

from __future__ import annotations

import numpy as np
import pytest

import cydist

F32_FUSED_CASES = [
    ("normal_sample_batch", {"mu": 0.0, "sigma": 1.0}),
    ("exponential_sample_batch", {"rate": 1.5}),
]


@pytest.mark.parametrize("func_name,kwargs", F32_FUSED_CASES)
def test_cydist_f32_sample_batch(func_name: str, kwargs: dict) -> None:
    fn = getattr(cydist, func_name)
    out = np.zeros(256, dtype=np.float32)
    fn(out, seed=42, **kwargs)
    assert out.dtype == np.float32
    assert np.all(np.isfinite(out))


@pytest.mark.parametrize("func_name,kwargs", F32_FUSED_CASES)
def test_cydist_f64_still_default(func_name: str, kwargs: dict) -> None:
    fn = getattr(cydist, func_name)
    out = np.zeros(256, dtype=np.float64)
    fn(out, seed=42, **kwargs)
    assert out.dtype == np.float64
    assert np.all(np.isfinite(out))
