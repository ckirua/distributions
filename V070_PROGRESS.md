# v0.7.0 progress (Phase 5) — batch 2 next

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` (active development branch).

## Status

| Metric | Count |
|--------|------:|
| Codegen with Tier B batch path | **0 / 171** |
| Tier B wave 1 shipped | **0 / 5** (target) |
| cydist Python validation | **26 / 171** (inherited from v0.6.0) |
| Shim `std::span` | **no** |
| Batches complete | **2 / 6** |
| `tier_b_candidate` in registry | **20** |

## Completed batches

### Batch 0 — tier registry scaffold + baseline

- `Recipe.batch_fast` + `tools/codegen/batch_fast.py` hook registry (emit Tier B dispatch when set)
- Registry YAML supports optional `batch_fast` field (sync on codegen)
- `bench/bench_codegen.py`, `make bench-codegen`, `make bench-codegen-baseline`
- Frozen wave-1 Tier-A CSVs in `results/baseline-v0.6.0/` (poisson, gamma, beta, uniform, student-t @10M)

### Batch 1 — codegen bench sweep + tier registry

- Full Tier-A sweep: **171 / 171** parameterized codegen @ 1k / 100k / 10M → `results/codegen-tier-a/` (local, gitignored CSVs)
- `bench/rank_codegen_tiers.py` — ranks by `per_sample` @10M × traffic × family boost
- Registry: **`tier_b_candidate: true`** on top 20; `tier_b_skip_reason` on heuristic / hand-written / exact
- Makefile: `bench-codegen-sweep`, `bench-codegen-sweep-resume`

## Top `tier_b_candidate` (batch 1 rank)

| vault id | bench @10M | notes |
|----------|----------:|-------|
| `continuous-bernoulli` | 3331.07 | CDF/rejection-heavy |
| `borel` | 3200.88 | discrete infinite |
| `discrete-weibull` | 2179.87 | |
| `beta` | 97.26 | wave-1 default |
| `students-t` | 87.75 | wave-1 default |
| `generalized-hyperbolic` | 102.78 | high-traffic |
| `gamma` | 48.40 | wave-1 default |
| `chi-squared` | 47.61 | high-traffic |

Full list of 20 tagged ids in `.vault/_meta/registry.yaml`.

## Wave-1 reference @10M (baseline-v0.6.0)

| vault id | bench @10M | tier_b_candidate |
|----------|----------:|:----------------:|
| `poisson` | 39.47 | no (fast already; promote in batch 2 anyway) |
| `gamma` | 48.40 | yes |
| `beta` | 97.26 | yes |
| `uniform` | 1.18 | no (Tier A already cheap) |
| `students-t` | 87.75 | yes |

## Tier B wave 1 queue (batch 2)

Default ship set (override with bench gate ≥1.5×): `poisson`, `gamma`, `beta`, `uniform`, `students-t`.

## Agent instructions

Continue batch 2 (Tier B wave 1). Push to `v0.7.0` only.
