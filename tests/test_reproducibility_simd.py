"""Tier B vs Tier C statistical equivalence for SIMD hand-written paths.

Tier B: ``detail::fast::*_sample_batch`` (scalar SplitMix64 / derived PCG).
Tier C: ``detail::simd::*_sample_batch`` (AVX2 when compiled with SIMD).

Requires ``build-simd/simd_tier_repro_test`` from ``make build-simd``.
"""

from __future__ import annotations

import subprocess
from pathlib import Path

import pytest

_REPO = Path(__file__).resolve().parents[1]
_SIMD_REPRO = _REPO / "build-simd" / "simd_tier_repro_test"


@pytest.mark.skipif(not _SIMD_REPRO.is_file(), reason="SIMD build not available")
def test_tier_b_vs_tier_c_simd() -> None:
    result = subprocess.run([str(_SIMD_REPRO)], capture_output=True, text=True, check=False)
    assert result.returncode == 0, (
        f"{_SIMD_REPRO} failed (exit {result.returncode})\n"
        f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
    )
