# v0.8.0 progress (Phase 6) — batch 1 next

Goal: **Tier B wave 3** (composition/transform) + **table sampler CDF polish** → merge as **`v1.0.0`**. See [`plan-v0.8.0.md`](plan-v0.8.0.md).

**Branch:** `v0.8.0` (from merged `main`, PR #3 merged).

## Status

| Metric | Count |
|--------|------:|
| Codegen Tier B (from v0.7.0) | **6 / 171** |
| Wave 3 shipped | **0** |
| Table sampler binary search / alias | **no** |
| Batches complete | **1 / 4** |
| Release tag | **—** |

## Completed batches

### Batch 0 — wave 3 scaffold + baseline ✅

- Branch `v0.8.0` pushed; PR #3 merged to `main`
- `bench_codegen --wave3` → `results/baseline-v0.8.0-wave3/` (11 candidates @10M)

## Next batches

### Batch 1 — table sampler binary search

- continuous-bernoulli, discrete-weibull fast paths

### Batch 2 — Tier B wave 3 (bench-gated)

- Composition/transform families (see plan queue)

### Batch 3 — integration sign-off

- `make test-all`, `results/baseline-v1.0.0/`

### Batch 4 — v1.0.0 release

- PR → `main`, tag `v1.0.0`

## Agent instructions

```
Continue v0.8.0 batch from V080_PROGRESS.md on branch v0.8.0.
Follow scripts/agent_v080_checklist.md and plan-v0.8.0.md.
```

Post-release work: [`plan-post-v1.md`](plan-post-v1.md).
