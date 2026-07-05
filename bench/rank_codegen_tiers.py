#!/usr/bin/env python3
"""Rank codegen Tier-A bench CSVs and tag tier_b_candidate in registry.yaml."""

from __future__ import annotations

import argparse
import csv
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]
REGISTRY = ROOT / ".vault" / "_meta" / "registry.yaml"
DEFAULT_CSV_DIR = ROOT / "results" / "codegen-tier-a"

BENCH_ALIAS = {
    "normal-gaussian": "normal",
    "students-t": "student-t",
}
REVERSE_ALIAS = {v: k for k, v in BENCH_ALIAS.items()}

# High-traffic cydist ids (usage boost in ranking).
HIGH_TRAFFIC = {
    "poisson", "gamma", "beta", "hypergeometric", "generalized-hyperbolic",
    "students-t", "chi-squared", "weibull", "log-normal", "pareto",
    "uniform", "cauchy", "laplace",
}

# Prefer transform / counting families for Tier-B wave 1–2.
PROMOTE_FAMILIES = {
    "poisson", "gamma", "beta", "uniform", "exponential", "normal",
    "geometric", "negative-binomial", "chi-squared", "weibull", "log-normal",
    "pareto", "cauchy", "laplace", "logistic", "gumbel", "erlang",
    "students-t", "student-t", "hypergeometric",
}

SKIP_TIERS = frozenset({"heuristic", "hand-written"})


def bench_id_for(vault_id: str) -> str:
    return BENCH_ALIAS.get(vault_id, vault_id)


def vault_id_for(bench_id: str) -> str:
    return REVERSE_ALIAS.get(bench_id, bench_id)


def load_per_sample_10m(csv_dir: Path) -> dict[str, float]:
    out: dict[str, float] = {}
    for path in sorted(csv_dir.glob("*.csv")):
        with path.open(newline="") as f:
            for row in csv.DictReader(f):
                if int(row["n"]) == 10_000_000:
                    out[path.stem] = float(row["per_sample"])
                    break
    return out


def score_entry(vault_id: str, per_sample: float, entry: dict) -> float | None:
    tier = entry.get("sampler_tier", "family")
    if tier in SKIP_TIERS:
        return None
    bench_id = bench_id_for(vault_id)
    traffic = 2.0 if vault_id in HIGH_TRAFFIC or bench_id in HIGH_TRAFFIC else 1.0
    family = 1.5 if vault_id in PROMOTE_FAMILIES or bench_id in PROMOTE_FAMILIES else 1.0
    return per_sample * traffic * family


def skip_reason(entry: dict) -> str | None:
    tier = entry.get("sampler_tier", "family")
    if tier == "heuristic":
        return "heuristic sampler (loc/scale fallback)"
    if tier == "hand-written":
        return "hand-written (separate Tier B track)"
    if tier == "exact":
        return "exact special-function path (profile before Tier B)"
    return None


def apply_ranking(
    csv_dir: Path,
    top_n: int = 20,
    dry_run: bool = False,
) -> tuple[list[tuple[str, float, float]], list[tuple[str, str]]]:
    per_sample = load_per_sample_10m(csv_dir)
    data = yaml.safe_load(REGISTRY.read_text())
    by_vid = {e["id"]: e for e in data["distributions"]}

    ranked: list[tuple[str, float, float]] = []
    skipped: list[tuple[str, str]] = []

    for bench_id, ps in per_sample.items():
        vid = vault_id_for(bench_id)
        entry = by_vid.get(vid)
        if entry is None:
            continue
        reason = skip_reason(entry)
        if reason:
            skipped.append((vid, reason))
            continue
        sc = score_entry(vid, ps, entry)
        if sc is None:
            skipped.append((vid, "not eligible"))
            continue
        ranked.append((vid, ps, sc))

    ranked.sort(key=lambda x: x[2], reverse=True)
    candidates = {vid for vid, _, _ in ranked[:top_n]}

    for entry in data["distributions"]:
        vid = entry["id"]
        if vid in candidates:
            entry["tier_b_candidate"] = True
            entry.pop("tier_b_skip_reason", None)
        else:
            entry.pop("tier_b_candidate", None)
            reason = skip_reason(entry)
            if reason:
                entry["tier_b_skip_reason"] = reason
            else:
                entry.pop("tier_b_skip_reason", None)

    if not dry_run:
        REGISTRY.write_text(
            yaml.dump(data, sort_keys=False, allow_unicode=True, default_flow_style=False)
        )

    return ranked, skipped


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--csv-dir", type=Path, default=DEFAULT_CSV_DIR)
    parser.add_argument("--top", type=int, default=20)
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    if not args.csv_dir.is_dir():
        print(f"csv dir missing: {args.csv_dir}", file=sys.stderr)
        return 1

    per = load_per_sample_10m(args.csv_dir)
    if not per:
        print(f"no @10M rows under {args.csv_dir}", file=sys.stderr)
        return 1

    ranked, skipped = apply_ranking(args.csv_dir, top_n=args.top, dry_run=args.dry_run)
    print(f"ranked {len(ranked)} eligible / {len(per)} benched / {len(skipped)} skipped")
    print("top candidates:")
    for vid, ps, sc in ranked[: args.top]:
        print(f"  {vid:30s} per_sample={ps:8.2f} score={sc:8.1f}")
    if not args.dry_run:
        print(f"registry: tier_b_candidate set on {min(args.top, len(ranked))} ids")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
