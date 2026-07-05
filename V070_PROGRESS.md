# v0.7.0 progress (Phase 5) — batch 6 next

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` (active development branch).

## Status

| Metric | Count |
|--------|------:|
| Codegen with Tier B batch path | **6 / 171** |
| Tier B wave 1+2 shipped | **6** (bench-gated) |
| cydist Python validation | **184 / 184** parameterized |
| Shim `std::span` | **yes** |
| Batches complete | **6 / 6** (batch 6 sign-off pending) |
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

### Batch 5 — Tier B wave 2 (bench-gated)

- Shipped @10M vs `results/codegen-tier-a/` Tier-A baseline:
  - **continuous-bernoulli** (13.4×) — precomputed inverse-CDF grid
  - **discrete-weibull** (46.5×) — precomputed PMF/CDF
  - **erlang** (2.97×) — gamma integer-shape fast path
  - **log-normal** (1.60×) — normal batch + `exp`
- Skipped: chi-squared (regression), students-t (1.18×), generalized-hyperbolic (1.05×)

## Agent instructions

Run batch 6 integration sign-off (`make test-all`, bench vs baseline, final tracker). Push to `v0.7.0` only.
