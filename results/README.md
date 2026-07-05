# Benchmark results

Per-distribution CSVs: `results/{dist}.csv`.

```bash
make build
python bench/sweep.py              # Phase-1 ISPC candidates (7 ids)
python bench/sweep.py --all --quick --skip-verify  # all 189 (10k + 100k)
python bench/sweep.py --all                        # all 189 (full batch sizes)
python bench/sweep.py --dist poisson # single distribution
python bench/aggregate_summary.py  # → results/summary.csv
```

## RNG policy

- Engine: **PCG32** (`include/distributions/rng.hpp`)
- One `Pcg32` per `sample_batch` call, seeded by caller
- ISPC batch kernels use **splitmix64 per lane** (statistically equivalent, not bit-identical to serial PCG)

## ISPC backend

Only the original Phase-1 subset has ISPC kernels; all other distributions bench C++ only. See `PHASE1_ISPC` in `bench/sweep.py`.

Historical Phase-1 decisions are preserved in committed CSVs where present.

After `make bench-all`, expect **189** per-distribution CSVs plus `summary.csv`.
