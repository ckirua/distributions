"""Statistical sanity tests: cydist samples vs scipy.stats reference (slow)."""

from __future__ import annotations

import numpy as np
import pytest

import cydist
from scipy_reference import SANITY_CASES, SanityCase, sample_scipy_reference
from statistical_compare import check_continuous, check_discrete

N_SAMPLES = 100_000
ALPHA = 0.01
CYDIST_SEED = 42
SCIPY_SEED = 12345


def _sample_cydist(case: SanityCase) -> np.ndarray:
    fn = getattr(cydist, case.py_func)
    dtype = np.int32 if case.discrete else np.float64
    out = np.empty(N_SAMPLES, dtype=dtype)
    fn(out, seed=CYDIST_SEED, **case.cydist_kwargs)
    return np.asarray(out)


def _sample_scipy(case: SanityCase) -> np.ndarray:
    rng = np.random.default_rng(SCIPY_SEED)
    return sample_scipy_reference(case, N_SAMPLES, rng)


def _ids(case: SanityCase) -> str:
    return case.vault_id


@pytest.mark.sanity
@pytest.mark.parametrize(
    "case",
    [pytest.param(c, marks=pytest.mark.xfail(reason="approximate sampler")) if c.xfail else c for c in SANITY_CASES],
    ids=_ids,
)
def test_matches_scipy(case: SanityCase) -> None:
    a = _sample_cydist(case)
    b = _sample_scipy(case)

    if case.discrete:
        ok, msg = check_discrete(a, b, ALPHA)
    else:
        ok, msg = check_continuous(a, b, ALPHA)

    assert ok, f"{case.vault_id}: {msg}"
