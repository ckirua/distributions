#!/usr/bin/env python3
"""Run C++ vs ISPC benchmarks and write results/{dist}.csv."""

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
SAMPLE_DUMP = BUILD / "sample_dump"
VERIFY = ROOT / "bench" / "verify.py"
RESULTS = ROOT / "results"
REGISTRY = ROOT / ".vault" / "_meta" / "registry.yaml"

BATCH_SIZES = [1_000, 100_000, 10_000_000]

# Original Phase-1 ISPC candidates (subset of full registry).
PHASE1_ISPC = {
    "bernoulli",
    "discrete-uniform",
    "binomial",
    "geometric",
    "exponential",
    "normal",
    "zipfmandelbrot",
}

DECISION_THRESHOLD = 1.3


def load_targets(all_dists: bool) -> list[tuple[str, bool]]:
    if not all_dists:
        return [(d, d in PHASE1_ISPC) for d in sorted(PHASE1_ISPC)]

    registry = yaml.safe_load(REGISTRY.read_text())["distributions"]
    alias = {"normal-gaussian": "normal", "students-t": "student-t"}
    out: list[tuple[str, bool]] = []
    seen: set[str] = set()
    for entry in registry:
        bench_id = alias.get(entry["id"], entry["id"])
        if bench_id in seen:
            continue
        seen.add(bench_id)
        out.append((bench_id, bench_id in PHASE1_ISPC))
    return sorted(out, key=lambda x: x[0])


def run_bench(dist: str, backend: str, n: int, seed: int = 42) -> dict[str, float]:
    out = subprocess.check_output(
        [str(RUN_BENCH), dist, backend, str(n), str(seed), "--csv"],
        text=True,
    ).strip()
    parts = out.split(",")
    return {
        "dist": parts[0],
        "backend": parts[1],
        "n": int(parts[2]),
        "seed": int(parts[3]),
        "median": float(parts[4]),
        "p99": float(parts[5]),
        "stddev": float(parts[6]),
        "per_sample": float(parts[7]),
    }


def verify_backends(dist: str, seed: int = 42, n: int = 100_000) -> bool:
    tmp = ROOT / "build" / "verify_tmp"
    tmp.mkdir(exist_ok=True)
    a = tmp / f"{dist}_cpp.bin"
    b = tmp / f"{dist}_ispc.bin"
    subprocess.check_call([str(SAMPLE_DUMP), dist, "cpp", str(seed), str(n), str(a)])
    subprocess.check_call([str(SAMPLE_DUMP), dist, "ispc", str(seed), str(n), str(b)])
    proc = subprocess.run(
        [sys.executable, str(VERIFY), str(a), str(b)],
        capture_output=True,
        text=True,
    )
    return proc.returncode == 0


def decide(rows: list[dict[str, float]]) -> tuple[str, float, str]:
    cpp = {r["n"]: r for r in rows if r["backend"] == "cpp"}
    ispc = {r["n"]: r for r in rows if r["backend"] == "ispc"}
    if not ispc:
        return "cpp", 1.0, "ispc not run or skipped"
    n_large = 10_000_000
    n_small = 1_000
    if n_large not in cpp or n_large not in ispc:
        return "cpp", 1.0, "missing large-n timings"
    ratio = cpp[n_large]["median"] / max(ispc[n_large]["median"], 1.0)
    if ratio > DECISION_THRESHOLD and ispc[n_small]["median"] <= cpp[n_small]["median"]:
        return "ispc", ratio, f"ISPC {ratio:.2f}x at n={n_large}, ok at n={n_small}"
    return "cpp", ratio, f"C++ kept (ratio={ratio:.2f} at n={n_large})"


def sweep_dist(
    dist: str,
    ispc_candidate: bool,
    skip_verify: bool,
    batch_sizes: list[int],
) -> None:
    rows: list[dict[str, float]] = []
    for n in batch_sizes:
        rows.append(run_bench(dist, "cpp", n))
        if ispc_candidate:
            rows.append(run_bench(dist, "ispc", n))

    verify_ok = True
    skip_reason = ""
    if ispc_candidate and not skip_verify:
        verify_ok = verify_backends(dist)
        if not verify_ok:
            skip_reason = "verify.py failed — ISPC rejected"
            rows = [r for r in rows if r["backend"] == "cpp"]

    winner, ratio, reason = decide(rows) if ispc_candidate and verify_ok else ("cpp", 1.0, skip_reason or "ISPC not a candidate")
    if not verify_ok:
        winner, ratio, reason = "cpp", 1.0, skip_reason

    RESULTS.mkdir(exist_ok=True)
    out_path = RESULTS / f"{dist}.csv"
    with out_path.open("w", newline="") as f:
        w = csv.DictWriter(
            f,
            fieldnames=[
                "dist",
                "backend",
                "n",
                "seed",
                "median",
                "p99",
                "stddev",
                "per_sample",
                "winner",
                "speedup_ratio",
                "decision_note",
            ],
        )
        w.writeheader()
        for r in rows:
            r = dict(r)
            r["winner"] = winner
            r["speedup_ratio"] = ratio
            r["decision_note"] = reason
            w.writerow(r)
    print(f"{dist}: winner={winner} ratio={ratio:.2f} — {reason}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dist", action="append", help="Limit to one dist id")
    parser.add_argument("--all", action="store_true", help="Sweep all 189 registry distributions")
    parser.add_argument(
        "--quick",
        action="store_true",
        help="Use 10k+100k batch sizes (faster full-registry sweep)",
    )
    parser.add_argument("--skip-verify", action="store_true")
    args = parser.parse_args()

    batch_sizes = [10_000, 100_000] if args.quick else BATCH_SIZES

    if not RUN_BENCH.exists():
        print("build/run_bench not found — run cmake --build build first", file=sys.stderr)
        return 1

    targets = load_targets(args.all)
    if args.dist:
        ids = set(args.dist)
        targets = [(d, c) for d, c in targets if d in ids]
        if not targets:
            print(f"unknown dist id(s): {', '.join(sorted(ids))}", file=sys.stderr)
            return 1

    for dist, ispc_c in targets:
        sweep_dist(dist, ispc_c, args.skip_verify, batch_sizes)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
