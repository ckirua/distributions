# v0.8.0 progress (Phase 6) — batch 3 next

Goal: **Tier B wave 3** (composition/transform) + **table sampler CDF polish** → merge as **`v1.0.0`**. See [`plan-v0.8.0.md`](plan-v0.8.0.md).

**Branch:** `v0.8.0`

## Status

| Metric | Count |
|--------|------:|
| Codegen Tier B | **9 / 171** |
| Wave 3 shipped | **3** (half-normal, inverse-gamma, beta-prime) |
| Table sampler binary search / alias | **yes** (batch 1) |
| Batches complete | **3 / 4** |
| Release tag | **—** |

## Completed batches

### Batch 0 — wave 3 scaffold + baseline ✅

- `bench_codegen --wave3` → `results/baseline-v0.8.0-wave3/` (Tier-A @10M)

### Batch 1 — table sampler binary search / alias ✅

- continuous-bernoulli **2.66×**, discrete-weibull **3.94×** vs v0.7.0 Tier B

### Batch 2 — Tier B wave 3 ✅

Shipped (≥1.5× vs Tier-A @10M):

| dist | Tier-A | Tier-B | speedup |
|------|-------:|-------:|--------:|
| half-normal | 37.25 | 23.76 | **1.57×** |
| inverse-gamma | 46.60 | 25.84 | **1.81×** |
| beta-prime | 97.40 | 58.75 | **1.66×** |

Skipped: weibull (1.09×), rayleigh (1.26×), pareto (regression), chi-squared/davis/scaled-inverse-chi-squared (df=3 gamma), student-t (1.19×).

Details: `results/baseline-v0.8.0-wave3-shipped/README.md`

## Next batches

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
