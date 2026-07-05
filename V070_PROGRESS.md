# v0.7.0 progress (Phase 5) — not started

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` (create after `v0.6.0` PR merge).

## Status

| Metric | Count |
|--------|------:|
| Codegen with Tier B batch path | **0 / 171** |
| Tier B wave 1 shipped | **0 / 5** (target) |
| cydist Python validation | **26 / 171** (inherited from v0.6.0) |
| Shim `std::span` | **no** |
| Batches complete | **0 / 6** |

## Blocked on

- [ ] PR `v0.6.0` → `main` merged

## Completed batches

_(none — start batch 0 after merge)_

## Tier B candidate queue (batch 1 fills this)

| vault id | bench @10M | batch_fast hook | shipped | batch |
|----------|----------:|-----------------|:-------:|------:|
| `poisson` | | | | |
| `gamma` | | | | |
| `beta` | | | | |
| `uniform` | | | | |
| `students-t` | | | | |

## Agent instructions

Do not start until `v0.6.0` is on `main`. Then batch 0 only.
