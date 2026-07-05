# v0.7.0 progress (Phase 5) — batch 1 next

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` (active development branch).

## Status

| Metric | Count |
|--------|------:|
| Codegen with Tier B batch path | **0 / 171** |
| Tier B wave 1 shipped | **0 / 5** (target) |
| cydist Python validation | **26 / 171** (inherited from v0.6.0) |
| Shim `std::span` | **no** |
| Batches complete | **1 / 6** |

## Completed batches

### Batch 0 — tier registry scaffold + baseline

- `Recipe.batch_fast` + `tools/codegen/batch_fast.py` hook registry (emit Tier B dispatch when set)
- Registry YAML supports optional `batch_fast` field (sync on codegen)
- `bench/bench_codegen.py`, `make bench-codegen`, `make bench-codegen-baseline`
- Frozen wave-1 Tier-A CSVs in `results/baseline-v0.6.0/` (poisson, gamma, beta, uniform, student-t @10M)

## Tier B candidate queue (batch 1 fills bench columns)

| vault id | bench @10M | batch_fast hook | shipped | batch |
|----------|----------:|-----------------|:-------:|------:|
| `poisson` | 39.47 | | | |
| `gamma` | 48.40 | | | |
| `beta` | 97.26 | | | |
| `uniform` | 1.18 | | | |
| `students-t` | 87.75 | | | |

## Agent instructions

Continue from batch 1 (full codegen bench sweep + tier registry). Push to `v0.7.0` only.
