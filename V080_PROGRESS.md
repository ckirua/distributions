# v0.8.0 progress (Phase 6) — batch 0 next

Goal: **Tier B wave 3** (composition/transform) + **table sampler CDF polish** → merge as **`v1.0.0`**. See [`plan-v0.8.0.md`](plan-v0.8.0.md).

**Branch:** `v0.8.0` (create from `main` after v0.7.0 merge).

## Status

| Metric | Count |
|--------|------:|
| Codegen Tier B (from v0.7.0) | **6 / 171** |
| Wave 3 shipped | **0** |
| Table sampler binary search | **no** |
| Batches complete | **0 / 4** |
| Release tag | **—** |

## v0.7.0 carry-over (must be on `main` before v0.8.0)

- Tier B: gamma, beta, continuous-bernoulli, discrete-weibull, erlang, log-normal
- cydist validation 184/184, shim `std::span`
- Codegen `template<typename Sample>` + default aliases (pending merge if uncommitted)

## Planned batches

### Batch 0 — wave 3 scaffold + baseline

- Branch `v0.8.0`, `bench_codegen --wave3`, trackers

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
