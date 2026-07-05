#!/usr/bin/env python3
"""Run C++ benchmarks and write results/{dist}.csv."""

from __future__ import annotations

import argparse
import csv
import subprocess
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
RUN_BENCH = BUILD / "run_bench"
RESULTS = ROOT / "results"
REGISTRY = ROOT / ".vault" / "_meta" / "registry.yaml"

BATCH_SIZES = [1_000, 100_000, 10_000_000]
QUICK_BATCH_SIZES = [10_000, 100_000]

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


def load_targets(all_dists: bool) -> list[str]:
    if not all_dists:
        return list(CORE_DIST)

    registry = yaml.safe_load(REGISTRY.read_text())["distributions"]
    alias = {"normal-gaussian": "normal", "students-t": "student-t"}
    out: list[str] = []
    seen: set[str] = set()
    for entry in registry:
        bench_id = alias.get(entry["id"], entry["id"])
        if bench_id in seen:
            continue
        seen.add(bench_id)
        out.append(bench_id)
    return sorted(out)


def run_bench(dist: str, n: int, seed: int = 42) -> dict[str, str]:
    proc = subprocess.run(
        [str(RUN_BENCH), dist, str(n), str(seed), "--csv"],
        capture_output=True,
        text=True,
        check=True,
    )
    line = proc.stdout.strip().splitlines()[-1]
    parts = line.split(",")
    keys = ["dist", "backend", "n", "seed", "median_cycles", "p99_cycles", "stddev_cycles", "per_sample"]
    return dict(zip(keys, parts, strict=True))


def sweep_dist(dist: str, batch_sizes: list[int]) -> None:
    rows = [run_bench(dist, n) for n in batch_sizes]
    RESULTS.mkdir(exist_ok=True)
    out_path = RESULTS / f"{dist}.csv"
    fieldnames = ["dist", "backend", "n", "seed", "median_cycles", "p99_cycles", "stddev_cycles", "per_sample"]
    with out_path.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        w.writerows(rows)
    per = next((r for r in rows if r["n"] == "10000000"), rows[-1])
    print(f"{dist}: per_sample={per['per_sample']} @ n={per['n']}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dist", action="append", help="Limit to one dist id")
    parser.add_argument("--all", action="store_true", help="Sweep all 189 registry distributions")
    parser.add_argument(
        "--quick",
        action="store_true",
        help="Use 10k+100k batch sizes (faster full-registry sweep)",
    )
    args = parser.parse_args()

    batch_sizes = QUICK_BATCH_SIZES if args.quick else BATCH_SIZES

    if not RUN_BENCH.exists():
        print("build/run_bench not found — run cmake --build build first", file=sys.stderr)
        return 1

    dists = load_targets(args.all)
    if args.dist:
        ids = set(args.dist)
        dists = [d for d in dists if d in ids]
        if not dists:
            print(f"unknown dist id(s): {', '.join(sorted(ids))}", file=sys.stderr)
            return 1

    for dist in dists:
        sweep_dist(dist, batch_sizes)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
