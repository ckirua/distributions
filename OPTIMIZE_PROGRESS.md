# Performance optimization progress

Goal: **single-thread** counter-RNG fast paths for the **13 hand-written** core (Tier B). Multi-thread parallel is **optional** (batches 7–8). See [`plan.md`](plan.md).

**Visible progress:** each batch is committed and pushed to [`v0.3.0`](https://github.com/ckirua/distributions/tree/v0.3.0). CI runs on every push.

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier B fast path | **9 / 13** |
| Hand-written with counter-RNG inner loop | **0 / 13** (SplitMix64 for Tier B) |
| `bench-core` geomean speedup @ 10M (1 core) | **~1.5×** on optimized subset |
| Parallel enabled (`batch_parallel.hpp`) | **no** |
| Family with parallel wrapper | **0 / 175** (optional batch 8) |
| Dead ISPC stub removed | **no** (gated OFF by default) |
| Required batches complete | **5 / 8** (0–4 done; 9 pending) |

Last push: batch 4 (counting discrete Tier-B fast paths).

## Next batch

**Batch 5** — Heavy discrete: `categorical`, `zipf`, `zipfmandelbrot`, `skellam`.

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

- SplitMix64 fast paths: bernoulli, discrete-uniform
- Speedups @10M: bernoulli **1.8×**, discrete-uniform **1.8×**
- `geometric`: Tier A only (SplitMix path failed scipy variance check)

### Batch 3 — Transform continuous (2)

- Blocked SplitMix64 + `log1p` (exponential); batched Box–Muller 2-up (normal)
- Speedups @10M: exponential **1.2×**, normal **1.7×**

### Batch 4 — Counting discrete (4)

- Binomial / beta-binomial: Bernoulli-sum with SplitMix64 (`n ≤ 256` trials)
- Negative-binomial: direct success-counting (replaces gamma–Poisson on fast path)
- Poisson-binomial: batched trial compares
- Speedups @10M: binomial **1.6×**, beta-binomial **1.3×**, poisson-binomial **1.7×**, negative-binomial **1.1×**

## Hand-written tracker (13)

| vault id | Tier B fast | engine | batch |
|----------|:-----------:|--------|------:|
| `bernoulli` | yes | SplitMix64 | 2 |
| `discrete-uniform` | yes | SplitMix64 | 2 |
| `geometric` | — | Tier A | 2 |
| `exponential` | yes | SplitMix64 | 3 |
| `normal-gaussian` | yes | SplitMix64 + Box–Muller 2-up | 3 |
| `binomial` | yes | SplitMix64 Bernoulli-sum | 4 |
| `negative-binomial` | yes | SplitMix64 counting | 4 |
| `beta-binomial` | yes | SplitMix64 + beta/gamma | 4 |
| `poisson-binomial` | yes | SplitMix64 | 4 |
| `categorical` | — | — | 5 |
| `zipf` | — | — | 5 |
| `zipfmandelbrot` | — | — | 5 |
| `skellam` | — | — | 5 |

## Baseline @ 10M → current (cycles/sample)

| bench id | baseline | current | speedup |
|----------|----------:|--------:|--------:|
| bernoulli | 1.21 | 0.67 | 1.8× |
| discrete-uniform | 1.57 | 0.87 | 1.8× |
| exponential | 20.97 | 17.77 | 1.2× |
| normal | 39.35 | 22.94 | 1.7× |
| binomial | 44.44 | 28.14 | 1.6× |
| negative-binomial | 112.52 | 102.68 | 1.1× |
| beta-binomial | 198.01 | 147.05 | 1.3× |
| poisson-binomial | 8.21 | 4.77 | 1.7× |

Full CSVs: `results/baseline-v0.2.0/*.csv`

## Agent instructions

Read [`scripts/agent_optimize_checklist.md`](scripts/agent_optimize_checklist.md). One batch per session; **always push** to `v0.3.0` after tests pass. Do **not** implement `batch_parallel.hpp` before batch 7.
