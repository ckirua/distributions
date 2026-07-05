"""Tests for batch_fast codegen scaffold (v0.7.0 batch 0)."""

from __future__ import annotations

import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from codegen.batch_fast import BATCH_FAST_HOOKS, emit_sample_batch, resolve_batch_fast  # noqa: E402
from codegen.models import Recipe  # noqa: E402


def test_tier_a_sample_batch_without_hook() -> None:
    recipe = Recipe(
        vault_id="poisson",
        cpp_class="Poisson",
        folder="discrete/infinite",
        discrete=True,
        members=[("double", "mu", "4.0")],
        sample_body="return detail::sample_poisson(rng, mu_);",
    )
    body, includes = emit_sample_batch(recipe)
    assert "kFastThreshold" not in body
    assert "for (std::size_t i = 0; i < n; ++i)" in body
    assert includes == ()


def test_tier_b_sample_batch_with_hook() -> None:
    recipe = Recipe(
        vault_id="exponential",
        cpp_class="Erlang",
        folder="continuous/semi_infinite",
        discrete=False,
        members=[("double", "rate", "1.0")],
        sample_body="return detail::sample_exponential(rng, rate_);",
        batch_fast="exponential",
    )
    body, includes = emit_sample_batch(recipe)
    assert "kFastThreshold" in body
    assert "exponential_sample_batch" in body
    assert "distributions/detail/fast/exponential.hpp" in includes


def test_unknown_batch_fast_raises() -> None:
    recipe = Recipe(
        vault_id="gamma",
        cpp_class="Gamma",
        folder="continuous/semi_infinite",
        discrete=False,
        batch_fast="gamma",
    )
    with pytest.raises(ValueError, match="unknown batch_fast"):
        resolve_batch_fast(recipe)


def test_batch_fast_hook_registry_non_empty() -> None:
    assert "exponential" in BATCH_FAST_HOOKS
    assert "normal" in BATCH_FAST_HOOKS
