# v0.8.0 plan (Phase 6) — Tier B wave 3 + table sampler polish → **v1.0.0**

**Prerequisite:** Phase 5 merged to `main` ([`v0.7.0`](plan-v0.7.0.md) PR). **Branch:** `v0.8.0` from `main`.

**Release:** merge `v0.8.0` → `main`, tag **`v1.0.0`**. Retire version branch after tag.

**Visible progress:** each batch commits and pushes to `v0.8.0`. Update [`V080_PROGRESS.md`](V080_PROGRESS.md). CI on push.

---

## Problem

Phase 5 promoted **6 / 171** codegen Tier B paths (geomean **4.95×** on those six). Two shipped samplers still do **O(grid) linear inverse-CDF** per draw after table precompute. Many **composition/transform** families (gamma/normal/uniform chains) remain Tier A despite cheap fast primitives.

---

## Goals

| Goal | Target |
|------|--------|
| Tier B wave 3 (composition/transform) | **+3–5** codegen ids, bench-gated ≥ **1.5×** @10M vs `codegen-tier-a` |
| Table sampler polish | **continuous-bernoulli**, **discrete-weibull**: binary search or alias on precomputed CDF |
| Codegen Tier B total | **≥ 9 / 171** (6 from v0.7.0 + 3+ new) |
| Repro / sanity | `make test-all` green every batch |
| Release | PR → `main`, tag **`v1.0.0`**, freeze `results/baseline-v1.0.0/` |

## Non-goals (Phase 6)

- Tier C SIMD for codegen
- `batch_parallel.hpp` / multi-thread (see [`plan-post-v1.md`](plan-post-v1.md))
- Profile-driven promotion of full `tier_b_candidate` queue (post-v1)
- Breaking cydist API or ABI
- float Tier B on codegen (post-v1 unless one id benches clearly)

---

## Design

### Wave 3 candidate queue (bench first, ship if ≥ 1.5×)

Priority: **one primitive, many structs** — reuse `gamma`, `normal`, `uniform`, `exponential` fast paths + transforms.

| vault id | Hook strategy | Notes |
|----------|---------------|-------|
| `chi-squared` | `gamma_sample_batch(shape=df/2, scale=2)` | Prefer even-integer df fast path; skip if df=3 regresses |
| `chi` | `sqrt` of chi-squared batch | composition |
| `weibull` | uniform block + `pow`/`log1p` vector loop | transform-dominated |
| `rayleigh` | uniform + `sqrt(-2 log u)` | transform |
| `pareto` | uniform + power | transform |
| `half-normal` | `normal_sample_batch` + `abs` | transform |
| `inverse-gamma` | `gamma_sample_batch` + reciprocal | composition |
| `scaled-inverse-chi-squared` | chi-squared + scale | composition |
| `davis` | gamma(df/2, 2) | same as chi-squared |
| `students-t` | improved fused normal+gamma batch | retry v0.7.0 near-miss (1.18×) |
| `beta-prime` | two gamma batches + ratio | if simple hook benches |

Run wave via `bench/bench_codegen.py --wave3` (add flag in batch 0). Baseline: `results/codegen-tier-a/`.

### Table sampler polish (batch 1)

| Sampler | Current | Target |
|---------|---------|--------|
| `continuous-bernoulli` | 512-point table, **linear** inverse CDF | **binary search** on cumulative weights (or alias if grid fixed per λ) |
| `discrete-weibull` | 101-point PMF, **linear** scan | **binary search** on cumulative |

Gate: ≥ **1.2×** additional speedup @10M vs current Tier B (not just vs Tier A).

### Promotion gate (unchanged)

| Gate | Action |
|------|--------|
| ≥ **1.5×** vs Tier-A @10M | Ship `batch_fast` + registry tag |
| < 1.5× or sanity regression | Keep Tier A |
| Tier B regression vs prior Tier B | Do not ship alias change |

---

## Batches

### Batch 0 — Infrastructure

| Task | Detail |
|------|--------|
| Branch `v0.8.0` | from merged `main` (includes v0.7.0) |
| `V080_PROGRESS.md`, `scripts/agent_v080_checklist.md` | trackers |
| `results/baseline-v0.7.0/` | already frozen; copy promoted CSVs if needed |
| `bench_codegen.py --wave3` | candidate list constant |
| Ensure v0.7.0 template codegen merged | `template<typename Sample>` on 171 structs |

**Commit:** `v0.8.0 batch 0: wave 3 scaffold + baseline`

---

### Batch 1 — Alias / CDF improvements (shipped Tier B)

| Task | Detail |
|------|--------|
| `detail::fast/continuous_bernoulli.hpp` | binary search (or alias) on precomputed grid |
| `detail::fast/discrete_weibull.hpp` | binary search on cumulative |
| Bench vs current Tier B @10M | document delta |
| Tests | smoke + sanity unchanged |

**Commit:** `v0.8.0 batch 1: table sampler binary search`

---

### Batch 2 — Tier B wave 3 (bench-gated)

| Task | Detail |
|------|--------|
| Add hooks in `batch_fast.py` + `detail::fast/` | composition/transform only |
| `Recipe.batch_fast` on passing ids | registry sync via codegen |
| Bench `--wave3` → `results/current/` | compare `codegen-tier-a` |
| Ship **3–5** ids meeting 1.5× gate | document skips |

**Commit:** `v0.8.0 batch 2: Tier B wave 3 (N samplers)`

---

### Batch 3 — Integration sign-off

| Task | Detail |
|------|--------|
| `make test-all` | smoke + sanity |
| Bench geomean | all promoted ids (v0.7.0 + v0.8.0) vs Tier-A |
| `results/baseline-v1.0.0/` | freeze Tier-A/Tier-B CSVs for release set |
| `V080_PROGRESS.md` final | |

**Commit:** `v0.8.0 batch 3: integration sign-off`

---

### Batch 4 — Release v1.0.0

| Task | Detail |
|------|--------|
| PR `v0.8.0` → `main` | full Phase 5+6 changelog |
| Tag **`v1.0.0`** on merge commit | `git tag v1.0.0` |
| GitHub release notes | metrics table from baselines |
| Delete `v0.8.0` branch | same hygiene as prior phases |

**Commit:** `v1.0.0: release sign-off` (optional docs-only on main after merge)

---

## Metrics tracker

Update [`V080_PROGRESS.md`](V080_PROGRESS.md):

| Metric | Start (v0.7.0) | Target (v1.0.0) |
|--------|---------------:|----------------:|
| Codegen Tier B | 6 / 171 | **≥ 9 / 171** |
| Table samplers optimized | linear scan | binary search / alias |
| Promoted geomean vs Tier-A | 4.95× (6 ids) | document all shipped |
| `make test-all` | green | green |

---

## Agent loop

See [`scripts/agent_v080_checklist.md`](scripts/agent_v080_checklist.md).

```
Continue v0.8.0 batch from V080_PROGRESS.md on branch v0.8.0.
Follow scripts/agent_v080_checklist.md and plan-v0.8.0.md.
```

Prior phases: [`V070_PROGRESS.md`](V070_PROGRESS.md), [`plan-v0.7.0.md`](plan-v0.7.0.md), [`plan-post-v1.md`](plan-post-v1.md).

---

## Branch setup

1. Merge PR `v0.7.0` → `main`
2. `git checkout main && git pull && git checkout -b v0.8.0`
3. Push `v0.8.0`, run batch 0
