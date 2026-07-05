# v0.7.0 progress (Phase 5) — batch 5 next

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` (active development branch).

## Status

| Metric | Count |
|--------|------:|
| Codegen with Tier B batch path | **2 / 171** |
| Tier B wave 1 shipped | **2 / 5** (bench-gated) |
| cydist Python validation | **184 / 184** parameterized |
| Shim `std::span` | **yes** |
| Batches complete | **5 / 6** |
| `tier_b_candidate` in registry | **20** |

## Completed batches

### Batch 0 — tier registry scaffold + baseline

- `Recipe.batch_fast` + `tools/codegen/batch_fast.py` hook registry
- `bench/bench_codegen.py`, `make bench-codegen`, `results/baseline-v0.6.0/`

### Batch 1 — codegen bench sweep + tier registry

- **171 / 171** Tier-A sweep; `rank_codegen_tiers.py`; top 20 `tier_b_candidate`

### Batch 2 — Tier B wave 1 (bench-gated)

- Shipped: **gamma** (2.85×), **beta** (1.64×) @10M vs baseline-v0.6.0
- Skipped: poisson, uniform, students-t (below 1.5× gate or regression)

### Batch 3 — cydist Python validation (all parameterized)

- Removed `CYDIST_PYTHON_VALIDATE` gate — all **184** parameterized `*_sample_batch` emit checks
- Vault overrides for stable skew `beta`, truncated-normal bounds, variance-gamma, bingham
- Coverage tests: `test_all_parameterized_cydist_have_python_checks`, `test_cydist_pyx_emits_checks_for_parameterized`

### Batch 4 — `std::span` at cydist shim

- `cydist/cydist_shim_span.hpp`: `checked_output_span` + `sample_batch` helper (length-checked via `std::span`)
- Codegen routes all 189 shim impls through `cydist_shim::sample_batch`; extern `"C"` ABI unchanged
- `tests/cpp/cydist_shim_span_test.cpp` + CMake target

## Agent instructions

Continue batch 5 (Tier B wave 2, bench-gated) or batch 6 (integration sign-off). Push to `v0.7.0` only.
