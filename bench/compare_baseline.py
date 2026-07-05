#!/usr/bin/env python3
"""Compare current bench-core CSVs against a baseline directory."""

from __future__ import annotations

import argparse
import csv
import math
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_BASELINE = ROOT / "results" / "baseline-v0.2.0"
DEFAULT_CURRENT = ROOT / "results" / "current"


def load_per_sample(path: Path) -> dict[int, float]:
    out: dict[int, float] = {}
    with path.open(newline="") as f:
        for row in csv.DictReader(f):
            out[int(row["n"])] = float(row["per_sample"])
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline", type=Path, default=DEFAULT_BASELINE)
    parser.add_argument("--current", type=Path, default=DEFAULT_CURRENT)
    parser.add_argument(
        "--max-regression",
        type=float,
        default=0.05,
        help="fail if per_sample regresses more than this fraction vs baseline",
    )
    parser.add_argument(
        "--n",
        type=int,
        default=10_000_000,
        help="batch size to compare (default 10M; use 1000 only for noisy Tier-A spot checks)",
    )
    parser.add_argument(
        "--geomean",
        action="store_true",
        help="print geomean speedup (baseline/current) at --n across matched CSVs",
    )
    args = parser.parse_args()

    if not args.baseline.is_dir():
        print(f"baseline dir missing: {args.baseline}", file=sys.stderr)
        return 1
    if not args.current.is_dir():
        print(f"current dir missing: {args.current}", file=sys.stderr)
        print("run: make bench-core", file=sys.stderr)
        return 1

    failed = False
    speedups: list[float] = []
    for path in sorted(args.baseline.glob("*.csv")):
        cur = args.current / path.name
        if not cur.is_file():
            print(f"SKIP {path.name}: no current csv")
            continue
        base = load_per_sample(path)
        now = load_per_sample(cur)
        if args.n not in base or args.n not in now:
            print(f"SKIP {path.name}: n={args.n} missing")
            continue
        ratio = now[args.n] / max(base[args.n], 1e-12)
        speedup = base[args.n] / max(now[args.n], 1e-12)
        speedups.append(speedup)
        if ratio > 1.0 + args.max_regression:
            print(
                f"FAIL {path.stem}: {ratio:.3f}x slower "
                f"({base[args.n]:.4g} -> {now[args.n]:.4g})"
            )
            failed = True
        else:
            tag = f" ({speedup:.2f}x faster)" if speedup > 1.01 else ""
            print(f"OK   {path.stem}: {ratio:.3f}x{tag}")

    if args.geomean and speedups:
        geo = math.exp(sum(math.log(s) for s in speedups) / len(speedups))
        print(f"GEOMEAN speedup @ n={args.n}: {geo:.3f}x ({len(speedups)} dists)")

    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
