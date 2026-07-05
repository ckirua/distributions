"""Codegen parameter validation coverage."""

from __future__ import annotations

import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from codegen.recipes import build_recipes  # noqa: E402


def test_codegen_validate_body_on_all_parameterized_recipes() -> None:
    registry = yaml.safe_load((ROOT / ".vault" / "_meta" / "registry.yaml").read_text())[
        "distributions"
    ]
    recipes = build_recipes(registry)
    missing = [r.vault_id for r in recipes.values() if r.members and not r.validate_body]
    assert not missing, f"missing validate_body: {missing[:10]}"
    assert len(missing) == 0
    with_members = sum(1 for r in recipes.values() if r.members)
    assert with_members >= 170
