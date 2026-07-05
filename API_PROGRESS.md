# Typed API progress (Phase 3)

Goal: **C++20 `Distribution` concept**, **`template<typename Sample>` + type aliases**, and **`float` paths** for ML/sim on the hand-written core. See [`plan-api.md`](plan-api.md).

**Branch:** [`v0.5.0`](https://github.com/ckirua/distributions/tree/v0.5.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written modeling `Distribution` concept (verified) | **13 / 13** |
| Hand-written with `template<typename Sample>` + alias | **2 / 13** |
| Hand-written with float Tier A | **0 / 13** |
| Required batches complete | **3 / 9** |

Last push: batch 2 (templated bernoulli + discrete-uniform).

## Next batch

**Batch 3** — `NormalDistribution<Sample>` + `ExponentialDistribution<Sample>`; float Tier A + mixed precision.

## Completed batches

### Batch 2 — Templated discrete (2)

- `BernoulliDistribution<Sample>` + `using Bernoulli = BernoulliDistribution<int>`
- `DiscreteUniformDistribution<Sample>` + `using DiscreteUniform = DiscreteUniformDistribution<int>`
- `static_assert(is_discrete_sample_v<Sample>)`; Tier B/C unchanged for `int` alias
- `int32_t` instantiations: Tier A loop only

### Batch 1 — Concept on hand-written core

- All 13 vault types verify `Distribution`; README public API section

### Batch 0 — Infrastructure

- `concepts.hpp`, trackers, branch `v0.5.0`

## Hand-written tracker (13)

| vault id | C++ type | templated | alias | batch |
|----------|----------|:---------:|:-----:|------:|
| `bernoulli` | `BernoulliDistribution<Sample>` | yes | `Bernoulli` | 2 |
| `discrete-uniform` | `DiscreteUniformDistribution<Sample>` | yes | `DiscreteUniform` | 2 |
| *(others)* | — | — | — | — |

## Agent instructions

Read [`scripts/agent_api_checklist.md`](scripts/agent_api_checklist.md). One batch per session; push to `v0.5.0` after tests pass.

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md).
