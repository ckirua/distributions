# v0.7.0 plan (Phase 5) — promote batch paths + validation completeness

**Prerequisite:** [`v0.6.0`](https://github.com/ckirua/distributions/tree/v0.6.0) merged — Phase 4 validation (C++ asserts on 184/184 parameterized, Python checks on 26 core ids). Phases 1–3 on `v0.3.0`–`v0.5.0` (Tier B/C hand-written, typed API, optional AVX-512).

**Branch:** `v0.7.0` from `v0.6.0` (after PR merge).

**Visible progress:** each batch commits and pushes to [`v0.7.0`](https://github.com/ckirua/distributions/tree/v0.7.0). Update [`V070_PROGRESS.md`](V070_PROGRESS.md) after every batch. CI runs on push.

---

## Problem

| Layer | 13 hand-written | 171 codegen (parameterized) |
|-------|-----------------|-----------------------------|
| `sample()` | tuned | family algorithms in `detail/` |
| C++ ctor asserts | yes | yes (batch 2) |
| Python `ValueError` | yes | 13 high-traffic only |
| Tier B `sample_batch` | 12/13 | **none** |
| Tier C SIMD | 6/13 | **none** |

The catalog is statistically correct but **batch throughput** is Tier A (serial PCG loop) for almost all codegen ids. Phase 5 **promotes** fast batch paths **selectively** using the same bench gates as Phases 1–2 — not a blanket copy of the 13 hand-written structs.

---

## Goals

| Goal | Target |
|------|--------|
| Bench-driven tier registry | Rank codegen ids; tag `batch_tier` candidates in registry |
| Tier B codegen promotion | ≥ **5** family samplers with `detail::fast::*_sample_batch` dispatch |
| Python validation completeness | All **171** parameterized cydist entry points raise `ValueError` |
| Shim ergonomics | `std::span` at cydist C shim boundary (optional batch) |
| Repro / sanity | `make test` + `make test-sanity` green every batch |

## Non-goals (Phase 5)

- Tier C SIMD for all 171 codegen distributions
- `template<typename Sample>` on all 171 structs
- Two-pass batch for every rejection sampler (profile per id)
- Multi-thread parallel (`batch_parallel.hpp`) — deferred from Phase 1
- Breaking cydist API or renaming `*_sample_batch` functions

---

## Design

### Tier registry (codegen)

Extend vault registry / codegen with optional **`batch_fast`** hook:

```yaml
# example registry field (batch 1)
batch_fast: poisson   # maps to detail::fast::poisson_sample_batch
```

Codegen `emit_header` when `batch_fast` set:

```cpp
void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
    if (n >= detail::kFastThreshold) {
        detail::fast::poisson_sample_batch(out, n, mu_, detail::batch_seed_from(rng));
        return;
    }
    for (std::size_t i = 0; i < n; ++i) out[i] = sample(rng);
}
```

Reuse existing `detail::fast/` primitives; add new ones only when bench proves ROI.

### Promotion gate (same as Phase 1–2)

| Gate | Action |
|------|--------|
| ≥ **1.5×** vs Tier A @ `n=10M` on target CPU | Ship Tier B for that id |
| < 1.5× or scipy sanity regression | Keep Tier A loop |
| Uniform / transform-dominated | Prefer Tier B |
| CDF / rejection-dominated | Tier A unless two-pass batch profiled |

Run `make bench-core` + compare against `results/baseline-v0.6.0/` (frozen at batch 0).

### Python validation

Change cydist emit: validate **all** specs with `params` (reuse `infer_cydist_python_checks`). No new rules — mirror C++ batch 2.

### `std::span` (optional)

C shim only: `void cydist_foo(..., std::span<double> out)` internally; keep C ABI `double* out, size_t n` for extern "C" if needed.

---

## Batches

### Batch 0 — Infrastructure

| Task | Detail |
|------|--------|
| Branch `v0.7.0` | from merged `v0.6.0` |
| `V070_PROGRESS.md`, `scripts/agent_v070_checklist.md` | trackers |
| `results/baseline-v0.6.0/` | freeze Tier-A codegen bench CSVs |
| Codegen scaffold | `Recipe.batch_fast: str \| None`, registry field optional |

**Commit:** `v0.7.0 batch 0: tier registry scaffold + baseline`

---

### Batch 1 — Bench sweep + tier registry

| Task | Detail |
|------|--------|
| Bench all 171 parameterized codegen @ 1k / 100k / 10M | extend `bench_core.py` or one-off sweep |
| Rank by cycles/sample; mark **tier_b_candidate** in registry | top ~20 by usage + bench |
| Document skip reasons | CDF-bound, heuristic-only, etc. |

**Acceptance:** registry YAML updated; progress tracker lists candidates.

**Commit:** `v0.7.0 batch 1: codegen bench sweep + tier registry`

---

### Batch 2 — Tier B wave 1 (3–5 samplers)

Default candidates (override if batch 1 bench differs):

| vault id | fast primitive | notes |
|----------|----------------|-------|
| `poisson` | `detail::fast` or existing poisson batch | λ nonnegative |
| `gamma` | `detail::fast::gamma_sample_batch` (add if missing) | shape/scale |
| `beta` | `detail::fast::beta` / sample_beta block | α,β > 0 |
| `uniform` | `detail::fast::uniform` / block uniforms | bounded |
| `students-t` | only if bench ≥ 1.5× | transcendental |

**Acceptance:** repro test Tier A vs B (statistical); bench CSV in `results/current/`.

**Commit:** `v0.7.0 batch 2: Tier B wave 1 (N samplers)`

---

### Batch 3 — cydist Python validation (all parameterized)

| Task | Detail |
|------|--------|
| Emit `infer_cydist_python_checks` for **every** spec with params | remove `CYDIST_PYTHON_VALIDATE` gate |
| Extend `tests/test_cydist_validation.py` | spot-check + coverage guard |
| Update `VALIDATION_PROGRESS.md` or fold into `V070_PROGRESS.md` | 171/171 Python |

**Commit:** `v0.7.0 batch 3: cydist validation all parameterized`

---

### Batch 4 — `std::span` at shim (optional)

| Task | Detail |
|------|--------|
| C++ shim helpers using `std::span` | length-checked |
| No change to Python/cython signatures | still NumPy `out` |

**Commit:** `v0.7.0 batch 4: span at cydist shim`

---

### Batch 5 — Tier B wave 2 (bench-gated)

- Promote next 3–5 ids from registry if batch 1 queue remains
- Optional: two-pass batch for **one** profiled heavy sampler (e.g. `generalized-hyperbolic`) if rejection dominates

**Commit:** `v0.7.0 batch 5: Tier B wave 2`

---

### Batch 6 — Integration sign-off

| Task | Detail |
|------|--------|
| Full `make test-all` | smoke + sanity |
| `make bench-core` vs baseline | document geomean on promoted ids |
| `V070_PROGRESS.md` final | |

**Commit:** `v0.7.0 batch 6: integration sign-off`

---

## Metrics tracker

Update [`V070_PROGRESS.md`](V070_PROGRESS.md):

| Metric | Start (v0.6.0) | Target |
|--------|---------------:|-------:|
| Codegen with Tier B batch | 0 / 171 | ≥ 5 (wave 1+2) |
| cydist Python validation | 26 / 171 | 171 / 171 |
| Shim uses `std::span` | no | optional yes |
| Bench baseline | `baseline-v0.6.0/` | `results/current/` |

---

## Agent loop

See [`scripts/agent_v070_checklist.md`](scripts/agent_v070_checklist.md).

```
Continue v0.7.0 batch from V070_PROGRESS.md on branch v0.7.0.
Follow scripts/agent_v070_checklist.md and plan-v0.7.0.md.
```

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md), [`API_PROGRESS.md`](API_PROGRESS.md), [`VALIDATION_PROGRESS.md`](VALIDATION_PROGRESS.md).

---

## Merge sequence (before v0.7.0)

1. **PR:** `v0.6.0` → `main` (Phases 3–4: typed API, AVX-512 spike, validation)
2. Merge PR
3. `git checkout -b v0.7.0 main` (or from updated `v0.6.0`)
4. Batch 0 on `v0.7.0`
