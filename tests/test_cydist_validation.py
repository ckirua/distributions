"""cydist Python parameter validation (Phase 4 batch 3)."""

from __future__ import annotations

import sys
from pathlib import Path

import numpy as np
import pytest

import cydist

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from codegen.constants import CYDIST_PYTHON_VALIDATE  # noqa: E402


@pytest.mark.parametrize("vault_id", sorted(CYDIST_PYTHON_VALIDATE))
def test_cydist_validate_set_non_empty(vault_id: str) -> None:
    assert vault_id


def test_bernoulli_invalid_p() -> None:
    out = np.empty(8, dtype=np.int32)
    with pytest.raises(ValueError, match=r"p must be in \[0, 1\]"):
        cydist.bernoulli_sample_batch(out, p=1.5)


def test_normal_invalid_sigma() -> None:
    out = np.empty(8, dtype=np.float64)
    with pytest.raises(ValueError, match="sigma must be positive"):
        cydist.normal_sample_batch(out, mu=0.0, sigma=0.0)


def test_exponential_invalid_rate() -> None:
    out = np.empty(8, dtype=np.float64)
    with pytest.raises(ValueError, match="rate must be positive"):
        cydist.exponential_sample_batch(out, rate=-1.0)


def test_poisson_invalid_mu() -> None:
    out = np.empty(8, dtype=np.int32)
    with pytest.raises(ValueError, match="mu must be non-negative"):
        cydist.poisson_sample_batch(out, mu=-0.1)


def test_gamma_invalid_shape() -> None:
    out = np.empty(8, dtype=np.float64)
    with pytest.raises(ValueError, match="shape must be positive"):
        cydist.gamma_sample_batch(out, shape=0.0, scale=1.0)


def test_generalized_hyperbolic_invalid_support() -> None:
    out = np.empty(8, dtype=np.float64)
    with pytest.raises(ValueError, match="a > \\|b\\|"):
        cydist.generalized_hyperbolic_sample_batch(out, p=0.0, a=1.0, b=2.0)


def test_discrete_uniform_invalid_interval() -> None:
    out = np.empty(8, dtype=np.int32)
    with pytest.raises(ValueError, match="low <= high"):
        cydist.discrete_uniform_sample_batch(out, low=5, high=1)


def test_categorical_negative_probs() -> None:
    out = np.empty(8, dtype=np.int32)
    with pytest.raises(ValueError, match="non-negative"):
        cydist.categorical_sample_batch(out, probs=np.array([-0.1, 0.6, 0.5]))


def test_hypergeometric_invalid_counts() -> None:
    out = np.empty(8, dtype=np.int32)
    with pytest.raises(ValueError, match="n_success <= M"):
        cydist.hypergeometric_sample_batch(out, M=10, n_success=12, N_draws=3)


def test_valid_params_still_sample() -> None:
    out = np.empty(64, dtype=np.float64)
    cydist.normal_sample_batch(out, mu=0.0, sigma=1.0, seed=1)
    assert np.all(np.isfinite(out))
