#!/usr/bin/env python3
"""Benchmark parameterized codegen distributions (Tier-A sample_batch loop)."""

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
REGISTRY = ROOT / ".vault" / "_meta" / "registry.yaml"
DEFAULT_OUT = ROOT / "results" / "current"
SWEEP_OUT = ROOT / "results" / "codegen-tier-a"

BATCH_SIZES = [1_000, 100_000, 10_000_000]
QUICK_BATCH_SIZES = [1_000, 100_000]

# Wave-1 Tier-B candidates (bench id = vault id except students-t → student-t).
WAVE1_CANDIDATES = [
    "poisson",
    "gamma",
    "beta",
    "uniform",
    "student-t",
]

WAVE2_CANDIDATES = [
    "chi-squared",
    "student-t",
    "generalized-hyperbolic",
    "continuous-bernoulli",
    "discrete-weibull",
]

# Wave 3 (v0.8.0): composition / transform families — bench before shipping.
WAVE3_CANDIDATES = [
    "chi-squared",
    "chi",
    "weibull",
    "rayleigh",
    "pareto",
    "half-normal",
    "inverse-gamma",
    "scaled-inverse-chi-squared",
    "davis",
    "student-t",
    "beta-prime",
]

BENCH_ALIAS = {
    "normal-gaussian": "normal",
    "students-t": "student-t",
}


def load_manual_ids() -> set[str]:
    sys.path.insert(0, str(ROOT / "tools"))
    from codegen.constants import MANUAL  # noqa: WPS433

    return set(MANUAL.keys())


def load_codegen_bench_ids() -> list[str]:
    manual = load_manual_ids()
    registry = yaml.safe_load(REGISTRY.read_text())["distributions"]
    sys.path.insert(0, str(ROOT / "tools"))
    from codegen.recipes import build_recipes  # noqa: WPS433

    recipes = build_recipes(registry)
    out: list[str] = []
    seen: set[str] = set()
    for entry in registry:
        vid = entry["id"]
        if vid in manual:
            continue
        recipe = recipes.get(vid)
        if recipe is None or not recipe.bench_ctor_args:
            continue
        bench_id = BENCH_ALIAS.get(vid, vid)
        if bench_id in seen:
            continue
        seen.add(bench_id)
        out.append(bench_id)
    return sorted(out)


def write_dist_csv(path: Path, rows: list[dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fieldnames = [
        "dist", "backend", "n", "seed", "median_cycles", "p99_cycles",
        "stddev_cycles", "per_sample",
    ]
    with path.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        w.writerows(rows)


def bench_dist(run_bench: Path, dist: str, sizes: list[int], seed: int = 42) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for n in sizes:
        proc = subprocess.run(
            [str(run_bench), dist, str(n), str(seed), "--csv"],
            capture_output=True,
            text=True,
            check=True,
        )
        line = proc.stdout.strip().splitlines()[-1]
        parts = line.split(",")
        keys = [
            "dist", "backend", "n", "seed", "median_cycles", "p99_cycles",
            "stddev_cycles", "per_sample",
        ]
        rows.append(dict(zip(keys, parts, strict=True)))
    return rows


def csv_has_10m(path: Path) -> bool:
    if not path.is_file():
        return False
    with path.open(newline="") as f:
        for row in csv.DictReader(f):
            if int(row["n"]) == 10_000_000:
                return True
    return False


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT)
    parser.add_argument("--run-bench", type=Path, default=RUN_BENCH)
    parser.add_argument("--quick", action="store_true", help="skip n=10M")
    parser.add_argument("--wave1", action="store_true", help="wave-1 Tier-B candidates only")
    parser.add_argument("--wave2", action="store_true", help="wave-2 Tier-B candidates only")
    parser.add_argument("--wave3", action="store_true", help="wave-3 composition/transform candidates (v0.8.0)")
    parser.add_argument("--dist", action="append", dest="dists", help="single dist (repeatable)")
    parser.add_argument(
        "--resume",
        action="store_true",
        help="skip dist if CSV already has n=10M row",
    )
    parser.add_argument(
        "--sweep",
        action="store_true",
        help=f"write all codegen CSVs to {SWEEP_OUT.name}/",
    )
    args = parser.parse_args()

    out_dir = SWEEP_OUT if args.sweep else args.out
    run_bench = args.run_bench
    if not run_bench.is_file():
        print(f"run_bench not found at {run_bench}; run make build first", file=sys.stderr)
        return 1

    sizes = QUICK_BATCH_SIZES if args.quick else BATCH_SIZES
    if args.dists:
        dists = args.dists
    elif args.wave1:
        dists = list(WAVE1_CANDIDATES)
    elif args.wave2:
        dists = list(WAVE2_CANDIDATES)
    elif args.wave3:
        dists = list(WAVE3_CANDIDATES)
    else:
        dists = load_codegen_bench_ids()

    done = 0
    for dist in dists:
        out_path = out_dir / f"{dist}.csv"
        if args.resume and csv_has_10m(out_path):
            print(f"{dist}: skipped (resume)")
            done += 1
            continue
        rows = bench_dist(run_bench, dist, sizes)
        write_dist_csv(out_path, rows)
        per = next((r for r in rows if r["n"] == "10000000"), rows[-1])
        print(f"{dist}: per_sample={per['per_sample']} @ n={per['n']}")
        done += 1

    print(f"wrote/kept {done} csv(s) under {out_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
