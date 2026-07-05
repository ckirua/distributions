# SIMD optimization progress (Phase 2)

Goal: **AVX2 Tier-C** vector `sample_batch` for the **13 hand-written** core on top of v0.3.0 Tier B. See [`plan-simd.md`](plan-simd.md).

**Branch:** [`v0.4.0`](https://github.com/ckirua/distributions/tree/v0.4.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier C SIMD path | **0 / 13** |
| AVX2 build gate (`DISTRIBUTIONS_ENABLE_SIMD`) | **yes** |
| Runtime cpuid guard (`detail/simd/config.hpp`) | **yes** |
| Geomean vs v0.3.0 Tier B @10M | **1.0×** (no SIMD samplers yet) |
| Geomean vs v0.2.0 @10M (cumulative) | **1.29×** (inherited) |
| Required batches complete | **2 / 7** |

Last push: batch 1 (AVX2 Philox uniform fill).

## Next batch

**Batch 2** — AVX2 bernoulli + discrete-uniform Tier C dispatch.

## Batch 6 decision gate

Target: geomean @10M vs **v0.2.0** baseline ≥ **2×** (currently 1.29×). Skip batch 7 (parallel) if met.

## Completed batches

### Batch 1 — Vector counter-RNG + uniforms

- `detail/simd/philox4x32_avx2.hpp`: Philox4x32-10 + AVX2 u01 block convert
- `detail/simd/uniform.hpp`: `fill_uniform01_avx2()` (falls back to scalar Philox)
- `tests/cpp/simd_uniform_test.cpp`: moments + bit-identical vs scalar Philox @1M

### Batch 0 — Infrastructure

- Branch `v0.4.0`; `DISTRIBUTIONS_ENABLE_SIMD=OFF` default, `-mavx2 -mfma` when ON
- `detail/simd/config.hpp`: `cpu_has_avx2()`, `tier_c_enabled()`
- `tests/cpp/simd_config_test.cpp` (ctest)
- Frozen Tier-B baseline: `results/baseline-v0.3.0/` (13 CSVs)
- `make build-simd`, `make bench-core-baseline`

## Hand-written tracker (13)

| vault id | Tier B (v0.3.0) | Tier C SIMD | batch |
|----------|:---------------:|:-----------:|------:|
| `bernoulli` | SplitMix64 | — | 2 |
| `discrete-uniform` | SplitMix64 | — | 2 |
| `geometric` | Tier A only | — | 2? |
| `exponential` | SplitMix64 | — | 3 |
| `normal-gaussian` | SplitMix64 | — | 3 |
| `binomial` | SplitMix64 | — | 4 |
| `negative-binomial` | SplitMix64 | — | 4 |
| `beta-binomial` | SplitMix64 | — | 4 |
| `poisson-binomial` | SplitMix64 | — | 4 |
| `categorical` | derived PCG | — | 5 |
| `zipf` | derived PCG | — | 5 |
| `zipfmandelbrot` | derived PCG | — | 5 |
| `skellam` | derived PCG | — | 5 |

## Reference timings (v0.3.0 Tier B @10M)

Frozen in `results/baseline-v0.3.0/`.

| bench id | cycles/sample |
|----------|-------------:|
| bernoulli | 0.68 |
| discrete-uniform | 0.86 |
| exponential | 17.75 |
| normal | 22.92 |
| binomial | 28.66 |
| negative-binomial | 102.89 |
| beta-binomial | 148.86 |
| poisson-binomial | 4.75 |
| categorical | 7.22 |
| zipf | 19.50 |
| zipfmandelbrot | 8.44 |
| skellam | 65.56 |
| geometric | 21.31 |

## Agent instructions

Read [`scripts/agent_simd_checklist.md`](scripts/agent_simd_checklist.md). One batch per session; push to `v0.4.0` after tests pass.

Phase 1 tracker: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md).
