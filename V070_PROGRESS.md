# v0.7.0 progress (Phase 5) — batch 3 next

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` (active development branch).

## Status

| Metric | Count |
|--------|------:|
| Codegen with Tier B batch path | **2 / 171** |
| Tier B wave 1 shipped | **2 / 5** (bench-gated) |
| cydist Python validation | **26 / 171** (inherited from v0.6.0) |
| Shim `std::span` | **no** |
| Batches complete | **3 / 6** |
| `tier_b_candidate` in registry | **20** |

## Completed batches

### Batch 0 — tier registry scaffold + baseline

- `Recipe.batch_fast` + `tools/codegen/batch_fast.py` hook registry (emit Tier B dispatch when set)
- Registry YAML supports optional `batch_fast` field (sync on codegen)
- `bench/bench_codegen.py`, `make bench-codegen`, `make bench-codegen-baseline`
- Frozen wave-1 Tier-A CSVs in `results/baseline-v0.6.0/`

### Batch 1 — codegen bench sweep + tier registry

- Full Tier-A sweep: **171 / 171** parameterized codegen @ 1k / 100k / 10M
- `bench/rank_codegen_tiers.py` — top 20 `tier_b_candidate` in registry
- Makefile: `bench-codegen-sweep`, `bench-codegen-sweep-resume`

### Batch 2 — Tier B wave 1 (bench-gated)

Shipped (≥ **1.5×** @10M vs `baseline-v0.6.0`):

| vault id | Tier A @10M | Tier B @10M | speedup | hook |
|----------|----------:|------------:|--------:|------|
| `gamma` | 48.40 | 16.99 | **2.85×** | Erlang sum (integer shape) |
| `beta` | 97.26 | 59.44 | **1.64×** | Erlang ratio (integer α, β) |

Skipped (gate not met or regression):

| vault id | reason |
|----------|--------|
| `poisson` | 1.14× best (SplitMix); below 1.5× gate |
| `uniform` | Tier B **slower** (already ~1.2 cycles/sample) |
| `students-t` | 1.19× with batched normal; below 1.5× gate |

New: `detail/fast/{gamma,beta,poisson,student_t}.hpp`; repro tests for gamma/beta.

## Agent instructions

Continue batch 3 (cydist Python validation 171/171). Push to `v0.7.0` only.
