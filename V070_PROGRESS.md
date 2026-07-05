# v0.7.0 progress (Phase 5) — batch 0 next

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` (active development branch).

## Status

| Metric | Count |
|--------|------:|
| Codegen with Tier B batch path | **0 / 171** |
| Tier B wave 1 shipped | **0 / 5** (target) |
| cydist Python validation | **26 / 171** (inherited from v0.6.0) |
| Shim `std::span` | **no** |
| Batches complete | **0 / 6** |

## Completed batches

_(none — start batch 0)_

## Tier B candidate queue (batch 1 fills this)

| vault id | bench @10M | batch_fast hook | shipped | batch |
|----------|----------:|-----------------|:-------:|------:|
| `poisson` | | | | |
| `gamma` | | | | |
| `beta` | | | | |
| `uniform` | | | | |
| `students-t` | | | | |

## Agent instructions

Continue from the next incomplete batch. Push to `v0.7.0` only.
