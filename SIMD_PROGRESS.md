# SIMD optimization progress (Phase 2)

Goal: **AVX2 Tier-C** vector `sample_batch` for the **13 hand-written** core on top of v0.3.0 Tier B. See [`plan-simd.md`](plan-simd.md).

**Branch:** [`v0.4.0`](https://github.com/ckirua/distributions/tree/v0.4.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier C SIMD path (dispatched) | **6 / 13** |
| Hand-written Tier C skipped (bench-bound) | **6** |
| AVX2 build gate (`DISTRIBUTIONS_ENABLE_SIMD`) | **yes** |
| Geomean vs v0.3.0 Tier B @10M (SIMD build) | **1.69×** |
| Geomean vs v0.2.0 @10M (cumulative) | **2.18×** |
| Required batches complete | **7 / 7** |

Last push: batch 6 (integration + bench sign-off).

## Batch 6 decision gate

Target: geomean @10M vs **v0.2.0** baseline ≥ **2×** — **met at 2.18×**. Batch 7 (parallel) **skipped**.

## Completed batches

### Batch 6 — Integration & sign-off

- `tests/cpp/simd_tier_repro_test.cpp` — Tier B vs Tier C moments + chi-square (7 kernels)
- `tests/test_reproducibility_simd.py` + `make test-simd`
- Full `make bench-core-simd` vs `baseline-v0.3.0` and `baseline-v0.2.0`
- **Fix:** poisson-binomial AVX threshold used `castpd_si256` (bit-cast bug); corrected in `detail/simd/poisson_binomial.hpp`, dispatch reverted to Tier B (correct path regressed ~3.5× vs scalar)

### Batch 5 — Heavy discrete (evaluated; Tier C skipped)

Trialed **4-stream SplitMix64**. CDF bisect / Knuth Poisson dominate. Tier B retained.

### Batch 4 — Counting discrete (2 shipped, 2 skipped)

- Tier C: **AVX2 4-wide Bernoulli trial compares** + **4 parallel output streams**
- Shipped: **binomial** (1.47×), **beta-binomial** (1.22×)
- Skipped: **negative-binomial** (~1.03× slower), **poisson-binomial** (AVX k=4 regressed when corrected)

### Batch 3 — Continuous (2)

- libmvec exponential **5.3×**, normal **4.2×** vs v0.3.0 Tier B

### Batch 2 — Trivial discrete (2)

- bernoulli **11.7×**, discrete-uniform **1.8×** vs v0.3.0 Tier B

### Batch 1 — Vector counter-RNG + uniforms

- Philox AVX2 infra (`fill_uniform01_avx2`)

### Batch 0 — Infrastructure

- CMake gate, cpuid, `results/baseline-v0.3.0/`

## Hand-written tracker (13)

| vault id | Tier B (v0.3.0) | Tier C SIMD | batch |
|----------|:---------------:|:-----------:|------:|
| `bernoulli` | SplitMix64 | **4-stream SplitMix** | 2 |
| `discrete-uniform` | SplitMix64 | **4-stream SplitMix** | 2 |
| `exponential` | SplitMix64 | **libmvec log1p** | 3 |
| `normal-gaussian` | SplitMix64 | **libmvec Box–Muller** | 3 |
| `binomial` | SplitMix64 | **AVX2 trial sum** | 4 |
| `beta-binomial` | SplitMix64 | **AVX2 trial sum** | 4 |
| `poisson-binomial` | SplitMix64 | skip (k=4 AVX regressed) | 4/6 |
| `negative-binomial` | SplitMix64 | skip (trial loop bound) | 4 |
| `categorical` | derived PCG | skip (alias bound) | 5 |
| `zipf` | derived PCG | skip (CDF bound) | 5 |
| `zipfmandelbrot` | derived PCG | skip (CDF bound) | 5 |
| `skellam` | derived PCG | skip (Poisson bound) | 5 |
| `geometric` | Tier A only | — | — |

## SIMD @10M vs v0.3.0 Tier B (cycles/sample)

| bench id | v0.3.0 | SIMD (final) | speedup |
|----------|-------:|-------------:|--------:|
| bernoulli | 0.68 | 0.06 | **11.7×** |
| discrete-uniform | 0.86 | 0.47 | **1.8×** |
| exponential | 17.75 | 3.33 | **5.3×** |
| normal | 22.92 | 5.44 | **4.2×** |
| binomial | 28.66 | 19.49 | **1.47×** |
| beta-binomial | 148.86 | 122.05 | **1.22×** |
| *(others — Tier B)* | | | ~1.0× |

Full CSVs: `results/baseline-v0.3.0/`, `results/current/` (SIMD build)

## Agent instructions

Phase 2 required batches **complete**. Optional batch 7 (parallel) only if geomean vs v0.2.0 < 2×.

Phase 1 tracker: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md).
