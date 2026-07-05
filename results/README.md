# Benchmark results

Per-distribution CSVs: `results/{dist}.csv` (gitignored except `summary.csv` and `baseline-v0.2.0/`).

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

Legacy Phase-1 ISPC kernels were archived to [`archive/ispc-phase1/`](../archive/ispc-phase1/) in optimize batch 9.

After `make bench-all`, expect per-distribution CSVs plus `summary.csv`.
