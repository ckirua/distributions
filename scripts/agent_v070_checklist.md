# Agent v0.7.0 batch checklist (Phase 5)

**Prerequisite:** Phase 4 merged to `main` (complete). Old version branches (`v0.2.0`–`v0.6.0`) deleted.

After each batch (see [`plan-v0.7.0.md`](../plan-v0.7.0.md)):

1. Read [`V070_PROGRESS.md`](../V070_PROGRESS.md) — implement **next batch only**.
2. Build & test:
   ```bash
   make codegen && make build && make test && make test-sanity
   ```
3. If batch touches Tier B or registry:
   ```bash
   make bench-core
   python bench/compare_baseline.py --baseline results/baseline-v0.6.0 --geomean
   ```
4. If batch touches SIMD (unlikely): `make build-simd && make test-simd`
5. Update [`V070_PROGRESS.md`](../V070_PROGRESS.md).
6. Commit on `v0.7.0`:
   ```bash
   git add -A
   git commit -m "v0.7.0 batch N: <short summary>"
   git push origin v0.7.0
   ```

## Scope rules

- **Do** promote Tier B via `Recipe.batch_fast` + existing `detail::fast/` — one primitive, many structs.
- **Do** require ≥ **1.5×** @10M vs Tier A before shipping a fast path.
- **Do not** add Tier C SIMD to codegen in Phase 5 unless a batch explicitly says so.
- **Do not** template all 171 distributions.
- **Do not** break Tier A reproducibility or sanity tests.
- **Do** keep `make test-sanity` green.

## Loop prompt (Cursor)

```
Continue v0.7.0 batch from V070_PROGRESS.md on branch v0.7.0.
Follow scripts/agent_v070_checklist.md and plan-v0.7.0.md.
```

Progress: commits to `v0.7.0` + CI on push.
