# Serial Tier-A baseline (v0.2.0)

Per-distribution CSVs from `make bench-core` before Tier-B fast paths land.

Columns match `run_bench --csv`: median/p99 cycles (net of buffer touch), `per_sample` cycles.

Regenerate:

```bash
make build
make bench-core          # 1k, 100k, 10M × 13 hand-written ids
make bench-core-quick    # 1k, 100k only
```

Compare after optimization (writes to `results/current/`, baseline stays frozen):

```bash
make bench-core
python bench/compare_baseline.py --geomean
python bench/compare_baseline.py --n 1000 --geomean   # optional; n=1k is noisy
```
