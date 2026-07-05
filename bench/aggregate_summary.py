#!/usr/bin/env python3
"""Aggregate results/{dist}.csv into results/summary.csv."""

from __future__ import annotations

import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RESULTS = ROOT / "results"


def main() -> None:
    rows: list[dict[str, str]] = []
    for path in sorted(RESULTS.glob("*.csv")):
        if path.name == "summary.csv":
            continue
        with path.open() as f:
            reader = csv.DictReader(f)
            dist_rows = list(reader)
        if not dist_rows:
            continue
        cpp_rows = [r for r in dist_rows if r["backend"] == "cpp"]
        if not cpp_rows:
            continue
        # Prefer 10M row; fall back to largest n for --quick sweeps.
        pick = next((r for r in cpp_rows if r["n"] == "10000000"), None)
        if pick is None:
            pick = max(cpp_rows, key=lambda r: int(r["n"]))
        rows.append(
            {
                "dist": pick["dist"],
                "winner": pick["winner"],
                "speedup_ratio": pick["speedup_ratio"],
                "cpp_median_10m": pick["median"],
                "n": pick["n"],
                "decision_note": pick["decision_note"],
            }
        )
    out = RESULTS / "summary.csv"
    with out.open("w", newline="") as f:
        w = csv.DictWriter(
            f,
            fieldnames=[
                "dist",
                "winner",
                "speedup_ratio",
                "cpp_median_10m",
                "n",
                "decision_note",
            ],
        )
        w.writeheader()
        w.writerows(rows)
    print(f"Wrote {out} ({len(rows)} rows)")


if __name__ == "__main__":
    main()
