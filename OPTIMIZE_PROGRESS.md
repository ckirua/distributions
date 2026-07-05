# Performance optimization progress

Goal: **single-thread** Tier-B fast `sample_batch` for the **13 hand-written** core. Multi-thread parallel is **optional** (batches 7–8, not implemented). See [`plan.md`](plan.md).

**Visible progress:** each batch is committed and pushed to [`v0.3.0`](https://github.com/ckirua/distributions/tree/v0.3.0). CI runs on every push.

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier B fast path | **12 / 13** |
| Hand-written Tier A only | **1** (`geometric`) |
| Parallel enabled (`batch_parallel.hpp`) | **no** |
| `bench-core` geomean speedup @10M (1 core) | **1.29×** |
| Dead ISPC stub removed | **yes** |
| Required batches complete | **8 / 8** |

Last push: batch 9 (ISPC cleanup).

## Next steps

Optimize track **complete**. Optional: batches 7–8 (parallel wrapper) if multi-core throughput is needed.

## Batch 6 decision gate

Geomean speedup @10M = **1.29×** (target was ~2×). **Batches 7–8 remain optional** — parallel wrapper only if large multi-core batches are needed later.

Tier-A correctness: `tests/test_reproducibility.py` (12 cases) + `geometric` unchanged @10M (1.003×).

## Completed batches

### Batch 9 — Cleanup

- Archived `ispc/` → `archive/ispc-phase1/`
- Removed `DISTRIBUTIONS_ENABLE_ISPC` CMake option and bench ISPC dispatch
- `bench/sweep.py`: C++-only sweep (no `PHASE1_ISPC`)
- Docs updated; `run_bench` usage is `<dist> [n] [seed] [--csv]`

### Batch 6 — Integration

- `kFastThreshold` = 4096 documented in README + DEVELOPMENT
- `tests/test_reproducibility.py`: Tier A (chunked) vs Tier B (large batch), 12 dists
- `bench/compare_baseline.py`: current vs frozen baseline; geomean report
- `make bench-core` writes to `results/current/` (baseline frozen in `baseline-v0.2.0/`)

### Batches 0–5

See git history (`Optimize batch 0` … `Optimize batch 5`).

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
| bernoulli | 1.21 | 0.68 | 1.77× |
| discrete-uniform | 1.57 | 0.86 | 1.84× |
| exponential | 20.97 | 17.75 | 1.18× |
| normal | 39.35 | 22.92 | 1.72× |
| binomial | 44.44 | 28.66 | 1.55× |
| negative-binomial | 112.52 | 102.89 | 1.09× |
| beta-binomial | 198.01 | 148.86 | 1.33× |
| poisson-binomial | 8.21 | 4.75 | 1.73× |
| categorical | 6.97 | 7.22 | ~1.0× |
| zipf | 20.37 | 19.50 | 1.04× |
| zipfmandelbrot | 8.42 | 8.44 | ~1.0× |
| skellam | 66.69 | 65.56 | 1.02× |
| geometric | 21.24 | 21.31 | ~1.0× |

Full CSVs: baseline `results/baseline-v0.2.0/`, current `results/current/`

## Agent instructions

Read [`scripts/agent_optimize_checklist.md`](scripts/agent_optimize_checklist.md). Optimize track is complete on `v0.3.0`.
