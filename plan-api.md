# Typed API plan (Phase 3)

**Prerequisite:** Phase 1 scalar Tier B (v0.3.0) and Phase 2 AVX2 Tier C (v0.4.0) complete. Geomean vs v0.2.0 **2.18×** @10M on the 13 hand-written core.

**Branch:** `v0.5.0` from `v0.4.0`.

**Visible progress:** each batch commits and pushes to [`v0.5.0`](https://github.com/ckirua/distributions/tree/v0.5.0). Update [`API_PROGRESS.md`](API_PROGRESS.md) after every batch. CI runs on push.

**Do not** remove Tier A/B/C dispatch or break existing `Bernoulli` / `Normal` spellings without type aliases.

---

## Goals

| Goal | Target |
|------|--------|
| Uniform C++ interface | C++20 `Distribution` concept on hand-written core |
| Sample templates | `template<typename Sample>` + **type aliases** for defaults (`using Normal = Normal<double>`) |
| Float / ML buffers | `float` Tier A (batch 2–3), optional float Tier B/C later |
| Backward compatibility | Existing structs, cydist `float64`/`int32`, benches unchanged until aliased |
| Repro / sanity | `make test` + `make test-sanity` green every batch |

## Non-goals (Phase 3)

- GPU / CUDA / DLPack
- SIMD for all 176 codegen distributions
- ISPC revival
- Mandatory AVX-512 (optional late batch)
- Breaking rename of `struct Bernoulli` without alias

---

## Design

### Template + alias (not duplicate structs)

```cpp
template <typename Sample = double>
struct Normal {
    using sample_type = Sample;
    [[nodiscard]] Sample sample(Pcg32& rng) const;
    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const;
    // ...
};

using Normal = Normal<double>;  // default — existing code keeps working
```

Discrete distributions default to **`int`** (later `int32_t` alias):

```cpp
template <typename Sample = int>
struct Bernoulli { /* ... */ };
using Bernoulli = Bernoulli<int>;
```

### Mixed precision (continuous float)

- **Store:** `Sample` (`float` or `double`)
- **Compute:** `compute_type_t<Sample>` = **`double`** unless batch notes otherwise
- Document in concept / README when float paths land (batch 3)

### RNG

- Tier A stays **`Pcg32`** in Phase 3 (no `template<typename RNG>` yet)
- Tier B/C remain seed-based helpers in `detail/fast/` and `detail/simd/`

### Python / cydist

- Batch 4+: optional `dtype=np.float32` on batch functions (keep float64 default)

---

## Batches

### Batch 0 — Infrastructure

| Task | Detail |
|------|--------|
| Branch `v0.5.0` | from `v0.4.0` |
| `concepts.hpp` | `Distribution`, `is_*_sample_v`, `sample_type_t`, `compute_type_t` |
| `tests/cpp/concepts_test.cpp` | compile-time + runtime concept checks |
| Trackers | `API_PROGRESS.md`, `scripts/agent_api_checklist.md` |

**Acceptance:** `make test` green; **no** behavior or API change to samplers.

**Commit:** `API batch 0: concept infrastructure + v0.5.0 branch`

---

### Batch 1 — Concept on all 13 hand-written

- Document public API in `include/distributions/README.md`
- Verify all 13 model `Distribution` (extend `concepts_test.cpp`)

**Commit:** `API batch 1: Distribution concept on hand-written core`

---

### Batch 2 — Template + alias, discrete (2)

- `Bernoulli<Sample>`, `DiscreteUniform<Sample>`; aliases `using Bernoulli = Bernoulli<int>`
- Tier A only; `double` path bit-identical to today

**Commit:** `API batch 2: templated bernoulli + discrete-uniform`

---

### Batch 3 — Template + alias, continuous (2)

- `Normal<Sample>`, `Exponential<Sample>`; default `double` aliases
- Float Tier A + mixed-precision policy

**Commit:** `API batch 3: templated normal + exponential (float Tier A)`

---

### Batch 4 — cydist float32 (batch 2–3 dists)

- `dtype=` kwarg or typed overloads for Python batch entry points

**Commit:** `API batch 4: cydist float32 paths`

---

### Batch 5 — Float Tier B (4 hot dists)

- Scalar fast paths for `float` where ROI clear

**Commit:** `API batch 5: float Tier B fast paths`

---

### Batch 6 — Float Tier C (optional SIMD)

- 8-wide float AVX2 where bench wins; skip regressions

**Commit:** `API batch 6: float Tier C SIMD (best-effort)`

---

### Batch 7 — Remaining hand-written templates

- Roll template + alias to other 9 hand-written types (`double`/`int` default)

**Commit:** `API batch 7: templated hand-written remainder`

---

### Batch 8 — Integration sign-off

- Typed repro tests; `API_PROGRESS.md` final; optional codegen `Distribution` static_asserts

**Commit:** `API batch 8: integration sign-off`

---

### Optional Batch 9 — AVX-512 spike

- CMake gate + 1–2 kernels; Intel HPC only

---

## Agent loop

See [`scripts/agent_api_checklist.md`](scripts/agent_api_checklist.md).

```
Continue API batch from API_PROGRESS.md on branch v0.5.0.
Follow scripts/agent_api_checklist.md and plan-api.md.
```

Phase 1–2 trackers: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md).
