# Parameter validation progress (Phase 4)

Goal: **debug asserts + optional Python checks** so invalid parameters fail fast instead of producing silent NaNs. See [`plan-validation.md`](plan-validation.md).

**Branch:** `v0.6.0`.

## Status

| Metric | Count |
|--------|------:|
| Hand-written with ctor validation | **13 / 13** |
| Codegen with `validate_body` | **171 / 171** (parameterized) |
| cydist Python `ValueError` | **184 / 184** (all parameterized; v0.7.0 batch 3) |
| Batches complete | **4 / 4** |

## Optional next (not batched yet)

- Two-pass batch for profiled heavy samplers (GH, etc.) — profiled in v0.7.0 batch 5, skipped (<1.5×)
- Extend Tier B to remaining `tier_b_candidate` registry queue

## Phase 5 (v0.7.0) — complete

See [`V070_PROGRESS.md`](V070_PROGRESS.md): 6 codegen Tier B paths, 184/184 cydist Python validation, shim `std::span`.

## Completed batches

### Batch 3 — cydist Python validation

- `CYDIST_PYTHON_VALIDATE` — 13 hand-written + 13 high-traffic codegen ids
- `infer_cydist_python_checks()` in `validation.py`; helpers emitted into `cydist.pyx`
- `tests/test_cydist_validation.py` — invalid-param `ValueError` + coverage parametrize

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

## cydist Python validation (184)

All parameterized cydist entry points (13 hand-written + 171 codegen) emit `ValueError` checks via `infer_cydist_python_checks()` (Phase 5 batch 3 on `v0.7.0`).

## Agent instructions

Phase 4 required batches complete. Follow-ups (Python 171/171, span, Tier B codegen) are in [`plan-v0.7.0.md`](plan-v0.7.0.md).

**Next:** Phase 5 complete on `v0.7.0` — merge to `main` (see [`V070_PROGRESS.md`](V070_PROGRESS.md)).
