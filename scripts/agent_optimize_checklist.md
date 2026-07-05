# Agent optimize batch checklist

After each performance batch (see [`plan.md`](../plan.md)):

1. Read [`OPTIMIZE_PROGRESS.md`](../OPTIMIZE_PROGRESS.md) — implement **next batch only**.
2. Build & test:
   ```bash
   make codegen && make build && make test && make test-sanity
   ```
3. Benchmark (when batch touches samplers or infra):
   ```bash
   make bench-core
   # after batch 0 lands:
   # python bench/compare_baseline.py --phase 0
   ```
4. Update [`OPTIMIZE_PROGRESS.md`](../OPTIMIZE_PROGRESS.md):
   - status counts
   - hand-written tracker checkmarks
   - completed batch section
5. Commit on `v0.3.0`:
   ```bash
   git add -A
   git commit -m "Optimize batch N: <short summary>"
   git push origin v0.3.0
   ```

## Scope rules

- **Do not** revive `ispc/kernels.ispc` as the primary SIMD strategy.
- **Do not** add `batch_parallel.hpp`, OpenMP, or threads before **batch 7** (optional).
- **Do not** add per-distribution SIMD in `tools/codegen/recipes.py` in the required path.
- **Do not** change serial `sample()` reproducibility on hand-written types (Tier A).
- **Do** keep `make test-sanity` green (fix or document xfail with reason).
- **Do** record bench numbers when claiming speedup (1 core, `@10M`).
- At **batch 6**: record geomean speedup; skip batches 7–8 unless explicitly requested.

## Loop prompt (Cursor)

```
Continue optimize batch from OPTIMIZE_PROGRESS.md on branch v0.3.0.
Follow scripts/agent_optimize_checklist.md and plan.md.
```

Progress is visible on GitHub via commits to `v0.3.0` and CI runs on push.
