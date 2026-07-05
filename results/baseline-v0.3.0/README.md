# Tier-B scalar baseline (v0.3.0)

Per-distribution CSVs from `make bench-core` after Phase 1 (scalar Tier-B fast paths), before SIMD Tier C.

Compare during Phase 2:

```bash
make bench-core
python bench/compare_baseline.py --baseline results/baseline-v0.3.0 --geomean
```

Regenerate (only when intentionally re-baselining Tier B):

```bash
make bench-core-baseline
```
