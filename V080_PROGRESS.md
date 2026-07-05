# v0.8.0 progress (Phase 6) — batch 4 next

Goal: merge as **`v1.0.0`**. See [`plan-v0.8.0.md`](plan-v0.8.0.md).

**Branch:** `v0.8.0`

## Status

| Metric | Value |
|--------|------:|
| Codegen Tier B | **9 / 171** |
| Wave 3 shipped | **3** |
| Table sampler polish | **yes** |
| Promoted geomean vs Tier-A @10M | **4.47×** (9 ids) |
| `make test-all` | **526 passed** |
| Batches complete | **3 / 4** |
| Release tag | **—** |

## Completed batches

### Batch 0 — wave 3 scaffold + baseline ✅

### Batch 1 — table sampler binary search / alias ✅

- continuous-bernoulli **2.66×**, discrete-weibull **3.94×** vs v0.7.0 Tier B

### Batch 2 — Tier B wave 3 ✅

- half-normal **1.57×**, inverse-gamma **1.81×**, beta-prime **1.66×** vs Tier-A

### Batch 3 — integration sign-off ✅

- `make test-all` green (526 passed, 11 xfailed, 3 xpassed)
- Frozen `results/baseline-v1.0.0/` (tier-a + tier-b for 9 promoted ids)
- **Geomean 4.47×** vs Tier-A @10M across all 9 promoted codegen Tier B paths

## Next batch

### Batch 4 — v1.0.0 release

- PR `v0.8.0` → `main`, tag **`v1.0.0`**

## Agent instructions

```
Continue v0.8.0 batch from V080_PROGRESS.md on branch v0.8.0.
Follow scripts/agent_v080_checklist.md and plan-v0.8.0.md.
```

Post-release: [`plan-post-v1.md`](plan-post-v1.md).
