"""Tests for codegen tier ranking (v0.7.0 batch 1)."""

from __future__ import annotations

import csv
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "bench"))

from rank_codegen_tiers import apply_ranking, load_per_sample_10m  # noqa: E402


def test_load_per_sample_10m(tmp_path: Path) -> None:
    path = tmp_path / "poisson.csv"
    with path.open("w", newline="") as f:
        w = csv.DictWriter(
            f,
            fieldnames=[
                "dist", "backend", "n", "seed", "median_cycles",
                "p99_cycles", "stddev_cycles", "per_sample",
            ],
        )
        w.writeheader()
        w.writerow({
            "dist": "poisson", "backend": "cpp", "n": "10000000", "seed": "42",
            "median_cycles": "1", "p99_cycles": "1", "stddev_cycles": "1",
            "per_sample": "40.0",
        })
    assert load_per_sample_10m(tmp_path) == {"poisson": 40.0}


def test_apply_ranking_dry_run(tmp_path: Path, monkeypatch) -> None:
    import rank_codegen_tiers as mod

    for name, ps in [("poisson", 40.0), ("gamma", 50.0), ("beta", 90.0)]:
        path = tmp_path / f"{name}.csv"
        with path.open("w", newline="") as f:
            w = csv.DictWriter(
                f,
                fieldnames=[
                    "dist", "backend", "n", "seed", "median_cycles",
                    "p99_cycles", "stddev_cycles", "per_sample",
                ],
            )
            w.writeheader()
            w.writerow({
                "dist": name, "backend": "cpp", "n": "10000000", "seed": "42",
                "median_cycles": "1", "p99_cycles": "1", "stddev_cycles": "1",
                "per_sample": str(ps),
            })

    ranked, _ = apply_ranking(tmp_path, top_n=2, dry_run=True)
    assert len(ranked) >= 2
    assert ranked[0][0] in {"poisson", "gamma", "beta"}
