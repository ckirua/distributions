# v0.7.0 progress (Phase 5) — complete

Goal: **bench-driven Tier B promotion** for selected codegen samplers + **complete cydist Python validation**. See [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Branch:** `v0.7.0` → merge to `main` (batch 7).

## Final status

| Metric | Result |
|--------|--------|
| Codegen with Tier B batch path | **6 / 171** |
| Tier B promoted (bench-gated) | **gamma, beta, continuous-bernoulli, discrete-weibull, erlang, log-normal** |
| Tier B geomean @10M vs Tier-A | **4.95×** (6 promoted) |
| cydist Python validation | **184 / 184** parameterized |
| Shim `std::span` | **yes** |
| `make test-all` | **522 passed** (8 C++ + 514 pytest) |
| Hand-written bench-core geomean vs v0.2.0 | **1.315×** (no regression) |
| Batches complete | **7 / 7** |

## Completed batches

### Batch 0 — tier registry scaffold + baseline

- `Recipe.batch_fast` + `tools/codegen/batch_fast.py` hook registry
- `bench/bench_codegen.py`, `make bench-codegen`, `results/baseline-v0.6.0/`

### Batch 1 — codegen bench sweep + tier registry

- **171 / 171** Tier-A sweep; `rank_codegen_tiers.py`; top 20 `tier_b_candidate`

### Batch 2 — Tier B wave 1 (bench-gated)

- Shipped: **gamma** (2.97×), **beta** (1.67×) @10M vs codegen-tier-a
- Skipped: poisson, uniform, students-t (below 1.5× gate or regression)

### Batch 3 — cydist Python validation (all parameterized)

- Removed `CYDIST_PYTHON_VALIDATE` gate — all **184** parameterized `*_sample_batch` emit checks
- Vault overrides for stable skew `beta`, truncated-normal bounds, variance-gamma, bingham

### Batch 4 — `std::span` at cydist shim

- `cydist/cydist_shim_span.hpp`; extern `"C"` ABI unchanged

### Batch 5 — Tier B wave 2 (bench-gated)

- Shipped: **continuous-bernoulli** (13.4×), **discrete-weibull** (46.8×), **erlang** (3.0×), **log-normal** (1.6×)
- Skipped: chi-squared, students-t, generalized-hyperbolic

### Batch 6 — integration sign-off

- `make test-all` green (522 passed)
- `make bench-core` vs `baseline-v0.2.0`: geomean **1.315×**
- Promoted codegen geomean vs `codegen-tier-a`: **4.95×**
- Frozen CSVs: `results/baseline-v0.7.0/`

### Batch 7 — merge to `main`

- PR [#3](https://github.com/ckirua/distributions/pull/3) (`v0.7.0` → `main`)

## Non-goals (deferred)

- Tier C SIMD for codegen (Phase 5 non-goal)
- Parallel `batch_parallel.hpp` (deferred from Phase 1)
- Further Tier B from remaining 20 `tier_b_candidate` ids (profile as needed)
