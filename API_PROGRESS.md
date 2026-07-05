# Typed API progress (Phase 3)

Goal: **C++20 `Distribution` concept**, **`template<typename Sample>` + type aliases**, and **`float` paths** for ML/sim on the hand-written core. See [`plan-api.md`](plan-api.md).

**Branch:** [`v0.5.0`](https://github.com/ckirua/distributions/tree/v0.5.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written modeling `Distribution` concept (verified) | **3 / 13** |
| Hand-written with `template<typename Sample>` + alias | **0 / 13** |
| Hand-written with float Tier A | **0 / 13** |
| Required batches complete | **1 / 9** |

Last push: batch 0 (concept infrastructure).

## Next batch

**Batch 1** — verify `Distribution` on all 13 hand-written; document public API in README.

## Completed batches

### Batch 0 — Infrastructure

- Branch `v0.5.0` from `v0.4.0`
- `include/distributions/concepts.hpp` — `Distribution`, sample traits, `compute_type_t`
- `tests/cpp/concepts_test.cpp` — bernoulli, discrete-uniform, normal
- `plan-api.md`, `API_PROGRESS.md`, `scripts/agent_api_checklist.md`
- **No sampler behavior change**

## Hand-written tracker (13)

| vault id | `Distribution` | templated | float | batch |
|----------|:--------------:|:---------:|:-----:|------:|
| `bernoulli` | yes | — | — | 0 |
| `discrete-uniform` | yes | — | — | 0 |
| `normal-gaussian` | yes | — | — | 0 |
| *(others)* | — | — | — | — |

## Agent instructions

Read [`scripts/agent_api_checklist.md`](scripts/agent_api_checklist.md). One batch per session; push to `v0.5.0` after tests pass.

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md).
