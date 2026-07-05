# Performance optimization progress

Goal: **single-thread** counter-RNG fast paths for the **13 hand-written** core (Tier B). Multi-thread parallel is **optional** (batches 7–8). See [`plan.md`](plan.md).

**Visible progress:** each batch is committed and pushed to [`v0.3.0`](https://github.com/ckirua/distributions/tree/v0.3.0). CI runs on every push.

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier B fast path | **0 / 13** |
| Hand-written with counter-RNG inner loop | **0 / 13** (primitives ready) |
| `bench-core` geomean speedup @ 10M (1 core) | **1.0×** (baseline recorded) |
| Parallel enabled (`batch_parallel.hpp`) | **no** |
| Family with parallel wrapper | **0 / 175** (optional batch 8) |
| Dead ISPC stub removed | **no** (gated OFF by default) |
| Required batches complete | **2 / 8** (0–1 done; 9 pending) |

Last push: batches 0–1 (baseline + counter RNG).

## Next batch

**Batch 2** — Trivial discrete fast paths: `bernoulli`, `discrete-uniform`, `geometric`.

## Batch 6 decision gate

After batch 6, record geomean speedup @10M on 1 core. **Skip batches 7–8** unless speedup is insufficient and large multi-core batches are needed.

## Completed batches

### Batch 0 — Infrastructure & bench truth

- Branch `v0.3.0`, `make bench-core`, baseline CSVs in `results/baseline-v0.2.0/`
- `DISTRIBUTIONS_ENABLE_ISPC=OFF` by default
- RNG tier docs, `bench/compare_baseline.py`

### Batch 1 — Counter RNG

- `detail/counter_rng.hpp` (Philox4x32-10), `detail/fast/uniform.hpp`
- `tests/cpp/counter_rng_test.cpp` (ctest)

## Hand-written tracker (13)

| vault id | Tier B fast | counter/SIMD | batch |
|----------|:-----------:|:------------:|------:|
| `bernoulli` | — | — | 2 |
| `discrete-uniform` | — | — | 2 |
| `geometric` | — | — | 2 |
| `exponential` | — | — | 3 |
| `normal-gaussian` | — | — | 3 |
| `binomial` | — | — | 4 |
| `negative-binomial` | — | — | 4 |
| `beta-binomial` | — | — | 4 |
| `poisson-binomial` | — | — | 4 |
| `categorical` | — | — | 5 |
| `zipf` | — | — | 5 |
| `zipfmandelbrot` | — | — | 5 |
| `skellam` | — | — | 5 |

## Baseline @ 10M (Tier A, cycles/sample)

| bench id | per_sample |
|----------|----------:|
| bernoulli | 1.21 |
| discrete-uniform | 1.57 |
| exponential | 20.97 |
| normal | 39.35 |
| binomial | 44.44 |

Full CSVs: `results/baseline-v0.2.0/*.csv`

## Agent instructions

Read [`scripts/agent_optimize_checklist.md`](scripts/agent_optimize_checklist.md). One batch per session; **always push** to `v0.3.0` after tests pass. Do **not** implement `batch_parallel.hpp` before batch 7.
