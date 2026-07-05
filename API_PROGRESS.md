# Typed API progress (Phase 3)

Goal: **C++20 `Distribution` concept**, **`template<typename Sample>` + type aliases**, and **`float` paths** for ML/sim on the hand-written core. See [`plan-api.md`](plan-api.md).

**Branch:** [`v0.5.0`](https://github.com/ckirua/distributions/tree/v0.5.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written modeling `Distribution` concept (verified) | **13 / 13** |
| Hand-written with `template<typename Sample>` + alias | **4 / 13** |
| Hand-written with float Tier A | **2 / 13** |
| Hand-written with float Tier B | **2 / 13** |
| Required batches complete | **6 / 9** |

Last push: batch 5 (float Tier B for normal/exponential; int32 Tier B/C for bernoulli/discrete-uniform).

## Next batch

**Batch 6** — Float Tier C SIMD (best-effort AVX2 for float paths).

## Completed batches

### Batch 5 — Float Tier B (4 hot dists)

- `detail::fast::normal_sample_batch(float*)` and `exponential_sample_batch(float*)` — compute in `double`, store `float`
- `NormalDistribution<float>` / `ExponentialDistribution<float>` dispatch Tier B at `kFastThreshold`
- `BernoulliDistribution<int32_t>` / `DiscreteUniformDistribution<int32_t>` now use Tier B/C (same as `int`)
- `tests/test_reproducibility_float.py` — Tier A vs Tier B for float32 normal/exponential

### Batch 4 — cydist float32 (FusedType)

- Cython `ContinuousOut` fused type (`float32` | `float64`) for `normal_sample_batch` and `exponential_sample_batch`
- Shim `*_f32` entry points call `NormalDistribution<float>` / `ExponentialDistribution<float>`
- Discrete batch entry points (bernoulli, discrete-uniform) remain `int32` only

### Batch 3 — Templated continuous (2)

- `NormalDistribution<Sample>` + `using Normal = NormalDistribution<double>`
- `ExponentialDistribution<Sample>` + `using Exponential = ExponentialDistribution<double>`
- **`float`** Tier A (Box–Muller / log1p in `compute_type_t` = `double`); **`double`** keeps Tier B/C

### Batch 2 — Templated discrete (2)

- `BernoulliDistribution`, `DiscreteUniformDistribution` + int aliases

### Batch 1 — Concept on hand-written core

### Batch 0 — Infrastructure

## Hand-written tracker (13)

| vault id | C++ template | alias | float Tier A | float Tier B | batch |
|----------|--------------|-------|:------------:|:------------:|------:|
| `bernoulli` | `BernoulliDistribution<Sample>` | `Bernoulli` | — | int32 Tier B/C | 2, 5 |
| `discrete-uniform` | `DiscreteUniformDistribution<Sample>` | `DiscreteUniform` | — | int32 Tier B/C | 2, 5 |
| `normal-gaussian` | `NormalDistribution<Sample>` | `Normal` | yes | yes | 3, 5 |
| `exponential` | `ExponentialDistribution<Sample>` | `Exponential` | yes | yes | 3, 5 |
| *(others)* | — | — | — | — |

## Agent instructions

Read [`scripts/agent_api_checklist.md`](scripts/agent_api_checklist.md). One batch per session; push to `v0.5.0` after tests pass.

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md).
