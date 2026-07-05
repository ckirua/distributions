# Performance optimization progress

Goal: **single-thread** counter-RNG fast paths for the **13 hand-written** core (Tier B). Multi-thread parallel is **optional** (batches 7–8). See [`plan.md`](plan.md).

**Visible progress:** each batch is committed and pushed to [`v0.3.0`](https://github.com/ckirua/distributions/tree/v0.3.0). CI runs on every push.

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier B fast path | **5 / 13** |
| Hand-written with counter-RNG inner loop | **0 / 13** (SplitMix64 for Tier B) |
| `bench-core` geomean speedup @ 10M (1 core) | **~1.5×** on batch-2/3 subset |
| Parallel enabled (`batch_parallel.hpp`) | **no** |
| Family with parallel wrapper | **0 / 175** (optional batch 8) |
| Dead ISPC stub removed | **no** (gated OFF by default) |
| Required batches complete | **4 / 8** (0–3 done; 9 pending) |

Last push: batch 3 (exponential + normal Tier-B fast paths).

## Next batch

**Batch 4** — Counting discrete: `binomial`, `negative-binomial`, `beta-binomial`, `poisson-binomial`.

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

### Batch 2 — Trivial discrete (3)

- SplitMix64 fast paths: bernoulli, discrete-uniform, geometric
- Speedups @10M: bernoulli **1.8×**, discrete-uniform **1.8×**
- `geometric`: Tier A only (SplitMix path failed scipy variance check; log-bound ~1.1×)

### Batch 3 — Transform continuous (2)

- `detail/fast/exponential.hpp` — blocked SplitMix64 + `log1p`
- `detail/fast/normal.hpp` — batched Box–Muller (2 normals per transform pair)
- Speedups @10M: exponential **1.2×**, normal **1.7×**

## Hand-written tracker (13)

| vault id | Tier B fast | engine | batch |
|----------|:-----------:|--------|------:|
| `bernoulli` | yes | SplitMix64 | 2 |
| `discrete-uniform` | yes | SplitMix64 | 2 |
| `geometric` | — | Tier A | 2 |
| `exponential` | yes | SplitMix64 | 3 |
| `normal-gaussian` | yes | SplitMix64 + Box–Muller 2-up | 3 |
| `binomial` | — | — | 4 |
| `negative-binomial` | — | — | 4 |
| `beta-binomial` | — | — | 4 |
| `poisson-binomial` | — | — | 4 |
| `categorical` | — | — | 5 |
| `zipf` | — | — | 5 |
| `zipfmandelbrot` | — | — | 5 |
| `skellam` | — | — | 5 |

## Baseline @ 10M → current (cycles/sample)

| bench id | baseline | current | speedup |
|----------|----------:|--------:|--------:|
| bernoulli | 1.21 | 0.67 | 1.8× |
| discrete-uniform | 1.57 | 0.87 | 1.8× |
| geometric | 21.23 | 18.52 | 1.1× |
| exponential | 20.97 | 17.77 | 1.2× |
| normal | 39.35 | 22.94 | 1.7× |

Full CSVs: `results/baseline-v0.2.0/*.csv`

## Agent instructions

Read [`scripts/agent_optimize_checklist.md`](scripts/agent_optimize_checklist.md). One batch per session; **always push** to `v0.3.0` after tests pass. Do **not** implement `batch_parallel.hpp` before batch 7.
