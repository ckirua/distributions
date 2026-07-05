# Agent SIMD batch checklist (Phase 2)

After each SIMD batch (see [`plan-simd.md`](../plan-simd.md)):

1. Read [`SIMD_PROGRESS.md`](../SIMD_PROGRESS.md) — implement **next batch only**.
2. Build & test (**SIMD off** — must always pass CI default):
   ```bash
   make codegen && make build && make test && make test-sanity
   ```
3. Build with SIMD and benchmark:
   ```bash
   make build-simd
   make bench-core-simd
   python bench/compare_baseline.py --baseline results/baseline-v0.3.0 --geomean
   ```
4. Update [`SIMD_PROGRESS.md`](../SIMD_PROGRESS.md):
   - status counts
   - hand-written Tier C checkmarks
   - completed batch section
   - geomean vs v0.3.0 and vs v0.2.0
5. Commit on `v0.4.0`:
   ```bash
   git add -A
   git commit -m "SIMD batch N: <short summary>"
   git push origin v0.4.0
   ```

## Scope rules

- **Do not** revive `archive/ispc-phase1/` as the primary strategy.
- **Do not** remove Tier B scalar paths — SIMD is Tier C with fallback.
- **Do not** add SIMD to `tools/codegen/recipes.py` in the required path.
- **Do not** enable `-ffast-math` without explicit batch scope and sanity audit.
- **Do not** add OpenMP/threads before **batch 7** (optional).
- **Do** keep `make test-sanity` green (SIMD off and on).
- **Do** add Tier B vs Tier C repro tests in batch 6 (or incrementally per batch).
- **Do** record bench numbers when claiming speedup (1 core, `@10M`).
- At **batch 6**: record geomean; skip batch 7 unless geomean vs v0.2.0 < 2×.

## Loop prompt (Cursor)

```
Continue SIMD batch from SIMD_PROGRESS.md on branch v0.4.0.
Follow scripts/agent_simd_checklist.md and plan-simd.md.
```

Progress is visible on GitHub via commits to `v0.4.0` and CI runs on push.
