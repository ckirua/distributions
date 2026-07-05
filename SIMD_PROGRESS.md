# SIMD optimization progress (Phase 2)

Goal: **AVX2 Tier-C** vector `sample_batch` for the **13 hand-written** core on top of v0.3.0 Tier B. See [`plan-simd.md`](plan-simd.md).

**Branch:** [`v0.4.0`](https://github.com/ckirua/distributions/tree/v0.4.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier C SIMD path | **7 / 13** |
| Hand-written Tier C skipped (bench-bound) | **5** |
| AVX2 build gate (`DISTRIBUTIONS_ENABLE_SIMD`) | **yes** |
| Geomean vs v0.3.0 Tier B @10M (SIMD build) | **1.64×** |
| Geomean vs v0.2.0 @10M (cumulative) | **~2.1×** |
| Required batches complete | **6 / 7** |

Last push: batch 4 (binomial, beta-binomial, poisson-binomial Tier C).

## Next batch

**Batch 6** — integration, Tier B vs Tier C repro tests, bench sign-off.

## Batch 6 decision gate

Target: geomean @10M vs **v0.2.0** baseline ≥ **2×** — **met at ~2.1× after batch 3**. Batch 7 (parallel) remains optional.

## Completed batches

### Batch 5 — Heavy discrete (evaluated; Tier C skipped)

Trialed **4-stream SplitMix64**. CDF bisect / Knuth Poisson dominate; vector RNG alone regresses or is neutral. Tier B retained for categorical, zipf, zipfmandelbrot, skellam.

### Batch 4 — Counting discrete (3 shipped, 1 skipped)

- Tier C: **AVX2 4-wide Bernoulli trial compares** + **4 parallel output streams**
- `detail/simd/binomial.hpp`, `detail/simd/beta_binomial.hpp`, `detail/simd/poisson_binomial.hpp`
- `negative-binomial`: 4-stream trial regressed ~1.03× — **skip** (variable-length loop bound)

| vault id | v0.3.0 | SIMD | speedup | decision |
|----------|-------:|-----:|--------:|----------|
| `binomial` | 28.66 | 19.50 | **1.47×** | ship |
| `beta-binomial` | 148.86 | 121.94 | **1.22×** | ship |
| `poisson-binomial` | 4.75 | 3.43 | **1.38×** | ship |
| `negative-binomial` | 102.89 | 106.19 | 1.03× slower | skip |

### Batch 3 — Continuous (2)

- Tier C: **single SplitMix64 + libmvec** (4-stream SplitMix trial regressed on exponential)
- Speedups @10M: exponential **5.2×**, normal **4.2×** vs v0.3.0 Tier B

### Batch 2 — Trivial discrete (2)

- Tier C: **4 parallel SplitMix64 streams**
- Speedups @10M: bernoulli **7.7×**, discrete-uniform **1.8×** vs v0.3.0 Tier B

### Batch 1 — Vector counter-RNG + uniforms

- `detail/simd/philox4x32_avx2.hpp`, `fill_uniform01_avx2` (infra)

### Batch 0 — Infrastructure

- Branch `v0.4.0`; CMake gate, cpuid, `results/baseline-v0.3.0/`

## Hand-written tracker (13)

| vault id | Tier B (v0.3.0) | Tier C SIMD | batch |
|----------|:---------------:|:-----------:|------:|
| `bernoulli` | SplitMix64 | **4-stream SplitMix** | 2 |
| `discrete-uniform` | SplitMix64 | **4-stream SplitMix** | 2 |
| `exponential` | SplitMix64 | **libmvec log1p** | 3 |
| `normal-gaussian` | SplitMix64 | **libmvec Box–Muller** | 3 |
| `binomial` | SplitMix64 | **AVX2 trial sum** | 4 |
| `beta-binomial` | SplitMix64 | **AVX2 trial sum** | 4 |
| `poisson-binomial` | SplitMix64 | **AVX2 k=4 trials** | 4 |
| `negative-binomial` | SplitMix64 | skip (trial loop bound) | 4 |
| `categorical` | derived PCG | skip (alias bound) | 5 |
| `zipf` | derived PCG | skip (CDF bound) | 5 |
| `zipfmandelbrot` | derived PCG | skip (CDF bound) | 5 |
| `skellam` | derived PCG | skip (Poisson bound) | 5 |
| `geometric` | Tier A only | — | — |

## SIMD @10M vs v0.3.0 Tier B (cycles/sample)

| bench id | v0.3.0 | SIMD | speedup |
|----------|-------:|-----:|--------:|
| bernoulli | 0.68 | 0.09 | **7.7×** |
| discrete-uniform | 0.86 | 0.47 | **1.8×** |
| exponential | 17.75 | 3.39 | **5.2×** |
| normal | 22.92 | 5.44 | **4.2×** |
| binomial | 28.66 | 19.50 | **1.47×** |
| beta-binomial | 148.86 | 121.94 | **1.22×** |
| poisson-binomial | 4.75 | 3.43 | **1.38×** |
| *(others — Tier B)* | | | ~1.0× |

Full CSVs: `results/baseline-v0.3.0/`, `results/current/` (SIMD build)

## Agent instructions

Read [`scripts/agent_simd_checklist.md`](scripts/agent_simd_checklist.md). One batch per session; push to `v0.4.0` after tests pass.

Phase 1 tracker: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md).
