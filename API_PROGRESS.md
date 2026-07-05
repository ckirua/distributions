# Typed API progress (Phase 3)

Goal: **C++20 `Distribution` concept**, **`template<typename Sample>` + type aliases**, and **`float` paths** for ML/sim on the hand-written core. See [`plan-api.md`](plan-api.md).

**Branch:** [`v0.5.0`](https://github.com/ckirua/distributions/tree/v0.5.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written modeling `Distribution` concept (verified) | **13 / 13** |
| Hand-written with `template<typename Sample>` + alias | **4 / 13** |
| Hand-written with float Tier A | **2 / 13** |
| Required batches complete | **4 / 9** |

Last push: batch 3 (templated normal + exponential, float Tier A).

## Next batch

**Batch 4** — cydist `float32` paths for bernoulli/discrete-uniform/normal/exponential batch entry points.

## Completed batches

### Batch 3 — Templated continuous (2)

- `NormalDistribution<Sample>` + `using Normal = NormalDistribution<double>`
- `ExponentialDistribution<Sample>` + `using Exponential = ExponentialDistribution<double>`
- **`float`** Tier A (Box–Muller / log1p in `compute_type_t` = `double`); **`double`** keeps Tier B/C

### Batch 2 — Templated discrete (2)

- `BernoulliDistribution`, `DiscreteUniformDistribution` + int aliases

### Batch 1 — Concept on hand-written core

### Batch 0 — Infrastructure

## Hand-written tracker (13)

| vault id | C++ template | alias | float Tier A | batch |
|----------|--------------|-------|:------------:|------:|
| `bernoulli` | `BernoulliDistribution<Sample>` | `Bernoulli` | — | 2 |
| `discrete-uniform` | `DiscreteUniformDistribution<Sample>` | `DiscreteUniform` | — | 2 |
| `normal-gaussian` | `NormalDistribution<Sample>` | `Normal` | yes | 3 |
| `exponential` | `ExponentialDistribution<Sample>` | `Exponential` | yes | 3 |
| *(others)* | — | — | — | — |

## Agent instructions

Read [`scripts/agent_api_checklist.md`](scripts/agent_api_checklist.md). One batch per session; push to `v0.5.0` after tests pass.

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md).
