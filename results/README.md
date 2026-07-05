# Benchmark results

Per-distribution CSVs land in `results/current/` (gitignored). Tracked baselines:

| Path | Contents |
|------|----------|
| [`baseline-v1.0.0/`](baseline-v1.0.0/) | **Codegen Tier B sign-off** — tier-a vs tier-b for 9 promoted ids |
| [`codegen-tier-a/`](codegen-tier-a/) | Tier-A reference @ 1k / 100k / 10M (171 parameterized codegen ids) |
| `baseline-v0.2.0/` … `baseline-v0.8.0-*` | Historical CSV snapshots (no README) |
| `summary.csv` | Aggregated sweep summary |

```bash
make build
make bench-core            # 13 hand-written ids → results/current/
make bench-all             # all 189 ids (C++ only)
python bench/sweep.py --dist poisson
python bench/aggregate_summary.py  # → results/summary.csv
python bench/compare_baseline.py --geomean
```

## RNG policy

Hand-written core uses Tier A/B dispatch — see [`include/distributions/README.md`](../include/distributions/README.md) and [`DEVELOPMENT.md`](../DEVELOPMENT.md).

- **Tier A:** serial `Pcg32` for `sample()` and small `sample_batch`
- **Tier B:** fast paths in `detail/fast/` when `n >= kFastThreshold` (4096)

Legacy Phase-1 ISPC kernels were archived to [`archive/ispc-phase1/`](../archive/ispc-phase1/).

After `make bench-all`, expect per-distribution CSVs plus `summary.csv`.
