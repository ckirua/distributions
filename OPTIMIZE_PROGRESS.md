# Performance optimization progress

Goal: **single-thread** Tier-B fast `sample_batch` for the **13 hand-written** core. Multi-thread parallel is **optional** (batches 7–8). See [`plan.md`](plan.md).

**Visible progress:** each batch is committed and pushed to [`v0.3.0`](https://github.com/ckirua/distributions/tree/v0.3.0). CI runs on every push.

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier B fast path | **12 / 13** |
| Hand-written Tier A only | **1** (`geometric`) |
| Parallel enabled (`batch_parallel.hpp`) | **no** |
| Required batches complete | **6 / 8** (0–5 done; 6 + 9 pending) |

Last push: batch 5 (heavy discrete Tier-B fast paths).

## Next batch

**Batch 6** — Integration: `kFastThreshold` docs, repro tests, bench sign-off vs baseline.

## Batch 6 decision gate

After batch 6, record geomean speedup @10M on 1 core. **Skip batches 7–8** unless speedup is insufficient.

## Completed batches

### Batch 5 — Heavy discrete (4)

- Categorical: alias table + derived-seed PCG batch
- Zipf / ZipfMandelbrot: CDF binary search + derived-seed PCG batch
- Skellam: dual Poisson with derived-seed PCG batch
- Speedups @10M: zipf **~1.03×**, skellam **~1.02×**; categorical/zipfmandelbrot neutral

### Batch 4 — Counting discrete (4)

- Binomial / beta-binomial: SplitMix64 Bernoulli-sum (`n ≤ 256` trials)
- Negative-binomial: SplitMix64 success counting
- Poisson-binomial: SplitMix64 batched trials
- Speedups @10M: binomial **1.6×**, beta-binomial **1.3×**, poisson-binomial **1.7×**, negative-binomial **1.1×**

### Batches 0–3

See git history (`Optimize batch 0` … `Optimize batch 3`).

## Hand-written tracker (13)

| vault id | Tier B | engine | batch |
|----------|:------:|--------|------:|
| `bernoulli` | yes | SplitMix64 | 2 |
| `discrete-uniform` | yes | SplitMix64 | 2 |
| `geometric` | — | Tier A | 2 |
| `exponential` | yes | SplitMix64 | 3 |
| `normal-gaussian` | yes | SplitMix64 + Box–Muller 2-up | 3 |
| `binomial` | yes | SplitMix64 | 4 |
| `negative-binomial` | yes | SplitMix64 | 4 |
| `beta-binomial` | yes | SplitMix64 | 4 |
| `poisson-binomial` | yes | SplitMix64 | 4 |
| `categorical` | yes | derived PCG | 5 |
| `zipf` | yes | derived PCG | 5 |
| `zipfmandelbrot` | yes | derived PCG | 5 |
| `skellam` | yes | derived PCG | 5 |

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
| categorical | 6.97 | 7.05 | ~1.0× |
| zipf | 20.37 | 19.83 | 1.03× |
| zipfmandelbrot | 8.42 | 8.42 | 1.0× |
| skellam | 66.69 | 65.48 | 1.02× |

Full CSVs: `results/baseline-v0.2.0/*.csv`

## Agent instructions

Read [`scripts/agent_optimize_checklist.md`](scripts/agent_optimize_checklist.md). One batch per session; **always push** to `v0.3.0` after tests pass.
