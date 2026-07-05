# Typed API progress (Phase 3)

Goal: **C++20 `Distribution` concept**, **`template<typename Sample>` + type aliases**, and **`float` paths** for ML/sim on the hand-written core. See [`plan-api.md`](plan-api.md).

**Branch:** [`v0.5.0`](https://github.com/ckirua/distributions/tree/v0.5.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written modeling `Distribution` concept (verified) | **13 / 13** |
| Hand-written with `template<typename Sample>` + alias | **0 / 13** |
| Hand-written with float Tier A | **0 / 13** |
| Required batches complete | **2 / 9** |

Last push: batch 1 (Distribution concept on all hand-written).

## Next batch

**Batch 2** — `Bernoulli<Sample>` + `DiscreteUniform<Sample>` with type aliases; Tier A only.

## Completed batches

### Batch 1 — Concept on hand-written core

- `Distribution` concept uses `const Dist&` (supports non-default-constructible types)
- `concepts_test.cpp` — all 13 vault types + `ZipfMandelbrot`
- Public API documented in `include/distributions/README.md`

### Batch 0 — Infrastructure

- Branch `v0.5.0`; `concepts.hpp`, trackers, no sampler behavior change

## Hand-written tracker (13)

| vault id | C++ type | sample | `Distribution` | templated | batch |
|----------|----------|--------|:--------------:|:---------:|------:|
| `bernoulli` | `Bernoulli` | `int` | yes | — | 1 |
| `discrete-uniform` | `DiscreteUniform` | `int` | yes | — | 1 |
| `binomial` | `Binomial` | `int` | yes | — | 1 |
| `categorical` | `Categorical` | `int` | yes | — | 1 |
| `beta-binomial` | `BetaBinomial` | `int` | yes | — | 1 |
| `poisson-binomial` | `PoissonBinomial` | `int` | yes | — | 1 |
| `zipf` | `Zipf` | `int` | yes | — | 1 |
| `zipfmandelbrot` | `ZipfMandelbrot` | `int` | yes | — | 1 |
| `geometric` | `Geometric` | `int` | yes | — | 1 |
| `negative-binomial` | `NegativeBinomial` | `int` | yes | — | 1 |
| `skellam` | `Skellam` | `int` | yes | — | 1 |
| `exponential` | `Exponential` | `double` | yes | — | 1 |
| `normal-gaussian` | `Normal` | `double` | yes | — | 1 |

## Agent instructions

Read [`scripts/agent_api_checklist.md`](scripts/agent_api_checklist.md). One batch per session; push to `v0.5.0` after tests pass.

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md).
