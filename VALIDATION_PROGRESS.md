# Parameter validation progress (Phase 4)

Goal: **debug asserts + optional Python checks** so invalid parameters fail fast instead of producing silent NaNs. See [`plan-validation.md`](plan-validation.md).

**Branch:** `v0.6.0`.

## Status

| Metric | Count |
|--------|------:|
| Hand-written with ctor validation | **13 / 13** |
| Codegen with `validate_body` | **171 / 171** (parameterized) |
| cydist Python validation | **no** |
| Batches complete | **3 / 4** |

## Optional next (not batched yet)

- Two-pass batch for profiled heavy samplers (GH, etc.)
- `std::span` at shim edges

## Completed batches

### Batch 2 — Codegen family validation

- `tools/codegen/validation.py` — infer `validate_body` from member names + vault overrides
- `finalize_recipe_validation()` wired in `build_recipes`
- `detail::assert_double_interval` for bounded supports
- All **171** parameterized codegen recipes emit ctor asserts; **5** parameterless unchanged
- `tests/test_validation_codegen.py` — coverage guard

### Batch 1 — Hand-written validation

- All 13 hand-written ctors call `detail/validate.hpp` asserts (debug-only)
- `tests/cpp/param_validate_smoke_test.cpp` — valid-parameter construction + one sample

### Batch 0 — Infrastructure

- `include/distributions/detail/validate.hpp`
- Codegen `Recipe.validate_body` + emit in ctor
- `plan-validation.md`, `VALIDATION_PROGRESS.md`, `scripts/agent_validation_checklist.md`

## Hand-written tracker (13)

| vault id | validation | batch |
|----------|:----------:|------:|
| `bernoulli` | `p ∈ [0,1]` | 1 |
| `discrete-uniform` | `low ≤ high` | 1 |
| `binomial` | `n ≥ 0`, `p ∈ [0,1]` | 1 |
| `categorical` | probs ≥ 0 | 1 |
| `beta-binomial` | `n ≥ 0`, `α,β > 0` | 1 |
| `poisson-binomial` | each `p ∈ [0,1]` | 1 |
| `zipf` | `N > 0`, `s > 0` | 1 |
| `zipfmandelbrot` | `N > 0`, `s > 0`, `q > -1` | 1 |
| `geometric` | `p ∈ (0,1]` | 1 |
| `negative-binomial` | `r > 0`, `p ∈ (0,1]` | 1 |
| `skellam` | `μ₁, μ₂ ≥ 0` | 1 |
| `normal-gaussian` | `σ > 0` | 1 |
| `exponential` | `rate > 0` | 1 |

## Agent instructions

Next batch: **3** — cydist Python `ValueError` for hand-written + high-traffic ids.
