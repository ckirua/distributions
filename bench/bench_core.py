#!/usr/bin/env python3
"""Benchmark the 13 hand-written (MANUAL) distributions at core batch sizes."""

from __future__ import annotations

import argparse
import csv
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
RUN_BENCH = BUILD / "run_bench"
DEFAULT_BASELINE = ROOT / "results" / "baseline-v0.2.0"
DEFAULT_OUT = ROOT / "results" / "current"

# vault id → bench id (matches tools/codegen/constants.py BENCH_ALIAS)
CORE_DIST = [
    "bernoulli",
    "discrete-uniform",
    "binomial",
    "categorical",
    "beta-binomial",
    "poisson-binomial",
    "zipf",
    "zipfmandelbrot",
    "geometric",
    "negative-binomial",
    "skellam",
    "exponential",
    "normal",
]

BATCH_SIZES = [1_000, 100_000, 10_000_000]
QUICK_BATCH_SIZES = [1_000, 100_000]


def run_row(dist: str, n: int, seed: int = 42) -> dict[str, str]:
    proc = subprocess.run(
        [str(RUN_BENCH), dist, "cpp", str(n), str(seed), "--csv"],
        capture_output=True,
        text=True,
        check=True,
    )
    line = proc.stdout.strip().splitlines()[-1]
    parts = line.split(",")
    if len(parts) != 8:
        raise RuntimeError(f"unexpected csv from run_bench: {line!r}")
    keys = ["dist", "backend", "n", "seed", "median_cycles", "p99_cycles", "stddev_cycles", "per_sample"]
    return dict(zip(keys, parts, strict=True))


def write_dist_csv(path: Path, rows: list[dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fieldnames = ["dist", "backend", "n", "seed", "median_cycles", "p99_cycles", "stddev_cycles", "per_sample"]
    with path.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        w.writerows(rows)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT)
    parser.add_argument("--quick", action="store_true", help="skip n=10M")
    parser.add_argument("--dist", action="append", dest="dists", help="single dist (repeatable)")
    args = parser.parse_args()

    if not RUN_BENCH.is_file():
        print(f"run_bench not found at {RUN_BENCH}; run make build first", file=sys.stderr)
        return 1

    sizes = QUICK_BATCH_SIZES if args.quick else BATCH_SIZES
    dists = args.dists or CORE_DIST

    for dist in dists:
        rows = [run_row(dist, n) for n in sizes]
        write_dist_csv(args.out / f"{dist}.csv", rows)
        per_10m = next((r for r in rows if r["n"] == "10000000"), rows[-1])
        print(f"{dist}: per_sample={per_10m['per_sample']} @ n={per_10m['n']}")

    print(f"wrote {len(dists)} csv(s) under {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
