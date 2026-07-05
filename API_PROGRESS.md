# Typed API progress (Phase 3)

Goal: **C++20 `Distribution` concept**, **`template<typename Sample>` + type aliases**, and **`float` paths** for ML/sim on the hand-written core. See [`plan-api.md`](plan-api.md).

**Branch:** [`v0.5.0`](https://github.com/ckirua/distributions/tree/v0.5.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written modeling `Distribution` concept (verified) | **13 / 13** |
| Hand-written with `template<typename Sample>` + alias | **13 / 13** |
| Hand-written with float Tier A | **2 / 13** |
| Hand-written with float Tier B | **2 / 13** |
| Hand-written with float Tier C | **2 / 13** |
| Required batches complete | **8 / 9** |

Last push: batch 7 (templated remaining 9 hand-written types).

## Next batch

**Batch 8** — Integration sign-off (typed repro tests, final tracker).

## Completed batches

### Batch 7 — Templated hand-written remainder (9)

- `BinomialDistribution`, `CategoricalDistribution`, `BetaBinomialDistribution`, `PoissonBinomialDistribution`
- `ZipfDistribution`, `ZipfMandelbrotDistribution`, `GeometricDistribution`, `NegativeBinomialDistribution`, `SkellamDistribution`
- Default aliases unchanged (`Binomial`, `Categorical`, …); Tier B/C dispatch via `is_discrete_sample_v` + `int32_t` buffer cast

### Batch 6 — Float Tier C SIMD

- 8-wide AVX2 `float` paths in `detail/simd/normal.hpp` and `exponential.hpp` (libmvec `*_ps`)
- `NormalDistribution<float>` / `ExponentialDistribution<float>` dispatch Tier C → Tier B fallback
- `simd_tier_repro_test`: `normal-f32` and `exponential-f32` Tier B vs Tier C checks

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

| vault id | C++ template | alias | float paths | batch |
|----------|--------------|-------|-------------|------:|
| `bernoulli` | `BernoulliDistribution<Sample>` | `Bernoulli` | int32 Tier B/C | 2, 5 |
| `discrete-uniform` | `DiscreteUniformDistribution<Sample>` | `DiscreteUniform` | int32 Tier B/C | 2, 5 |
| `binomial` | `BinomialDistribution<Sample>` | `Binomial` | int Tier B/C | 7 |
| `categorical` | `CategoricalDistribution<Sample>` | `Categorical` | int Tier B | 7 |
| `beta-binomial` | `BetaBinomialDistribution<Sample>` | `BetaBinomial` | int Tier B/C | 7 |
| `poisson-binomial` | `PoissonBinomialDistribution<Sample>` | `PoissonBinomial` | int Tier B | 7 |
| `zipf` | `ZipfDistribution<Sample>` | `Zipf` | int Tier B | 7 |
| `zipfmandelbrot` | `ZipfMandelbrotDistribution<Sample>` | `ZipfMandelbrot` | int Tier B | 7 |
| `geometric` | `GeometricDistribution<Sample>` | `Geometric` | Tier A only | 7 |
| `negative-binomial` | `NegativeBinomialDistribution<Sample>` | `NegativeBinomial` | int Tier B | 7 |
| `skellam` | `SkellamDistribution<Sample>` | `Skellam` | int Tier B | 7 |
| `normal-gaussian` | `NormalDistribution<Sample>` | `Normal` | float Tier A/B/C | 3, 5, 6 |
| `exponential` | `ExponentialDistribution<Sample>` | `Exponential` | float Tier A/B/C | 3, 5, 6 |

## Agent instructions

Read [`scripts/agent_api_checklist.md`](scripts/agent_api_checklist.md). One batch per session; push to `v0.5.0` after tests pass.

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md).
