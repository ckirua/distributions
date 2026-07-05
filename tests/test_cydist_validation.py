"""cydist Python parameter validation (Phase 4–5)."""

from __future__ import annotations

import sys
from pathlib import Path

import numpy as np
import pytest
import yaml

import cydist

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from codegen.emit import build_cydist_specs  # noqa: E402
from codegen.recipes import build_recipes  # noqa: E402
from codegen.validation import infer_cydist_python_checks  # noqa: E402


def _parameterized_specs():
    registry = yaml.safe_load((ROOT / ".vault" / "_meta" / "registry.yaml").read_text())[
        "distributions"
    ]
    recipes = build_recipes(registry)
    return [s for s in build_cydist_specs(registry, recipes) if s.params]


def test_all_parameterized_cydist_have_python_checks() -> None:
    specs = _parameterized_specs()
    missing = [
        s.vault_id
        for s in specs
        if not infer_cydist_python_checks(s.vault_id, s.params)
    ]
    assert not missing, f"missing python checks: {missing[:10]}"
    assert len(specs) >= 184


def test_cydist_pyx_emits_checks_for_parameterized() -> None:
    pyx = (ROOT / "cydist" / "cydist.pyx").read_text()
    specs = _parameterized_specs()
    missing_emit: list[str] = []
    for spec in specs:
        marker = f"def {spec.py_func}("
        start = pyx.find(marker)
        assert start != -1, spec.py_func
        chunk = pyx[start : start + 800]
        body = chunk.split("with nogil:")[0]
        if "_check_" not in body and "raise ValueError" not in body:
            missing_emit.append(spec.vault_id)
    assert not missing_emit, f"pyx missing _check_: {missing_emit[:10]}"


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


def test_weibull_invalid_shape() -> None:
    out = np.empty(8, dtype=np.float64)
    with pytest.raises(ValueError, match="shape must be positive"):
        cydist.weibull_sample_batch(out, shape=0.0, scale=1.0)


def test_valid_params_still_sample() -> None:
    out = np.empty(64, dtype=np.float64)
    cydist.normal_sample_batch(out, mu=0.0, sigma=1.0, seed=1)
    assert np.all(np.isfinite(out))
