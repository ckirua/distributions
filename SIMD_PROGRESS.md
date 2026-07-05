# SIMD optimization progress (Phase 2)

Goal: **AVX2 Tier-C** vector `sample_batch` for the **13 hand-written** core on top of v0.3.0 Tier B. See [`plan-simd.md`](plan-simd.md).

**Branch:** [`v0.4.0`](https://github.com/ckirua/distributions/tree/v0.4.0).

## Status

| Metric | Count |
|--------|------:|
| Hand-written with Tier C SIMD path | **4 / 13** |
| AVX2 build gate (`DISTRIBUTIONS_ENABLE_SIMD`) | **yes** |
| Geomean vs v0.3.0 Tier B @10M (SIMD build) | **1.65×** |
| Geomean vs v0.2.0 @10M (cumulative) | **2.12×** |
| Required batches complete | **4 / 7** |

Last push: batch 3 (exponential + normal Tier C via libmvec).

## Next batch

**Batch 4** — binomial, beta-binomial, negative-binomial, poisson-binomial.

## Batch 6 decision gate

Target: geomean @10M vs **v0.2.0** baseline ≥ **2×** — **met at 2.12× after batch 3**. Batch 7 (parallel) remains optional unless further gains are needed.

## Completed batches

### Batch 3 — Continuous (2)

- Tier C: **single SplitMix64 stream + 256 uniform block + libmvec vector math** (4-stream SplitMix trial regressed ~1.3× on exponential — rejected)
- `detail/simd/libmvec.hpp` — `_ZGVdN4v_log1p`, `_log`, `_cos`, `_sin`; CMake links `-lmvec` when SIMD on
- `detail/simd/exponential.hpp`, `detail/simd/normal.hpp`
- Speedups @10M (SIMD build): exponential **5.2×**, normal **4.2×** vs v0.3.0 Tier B

### Batch 2 — Trivial discrete (2)

- Tier C: **4 parallel SplitMix64 streams** (Philox trial regressed ~5× — rejected)
- `detail/simd/bernoulli.hpp`, `detail/simd/discrete_uniform.hpp`
- `make bench-core-simd` + compare vs `baseline-v0.3.0`
- Speedups @10M (SIMD build): bernoulli **9.5×**, discrete-uniform **1.7×** vs v0.3.0 Tier B

### Batch 1 — Vector counter-RNG + uniforms

- `detail/simd/philox4x32_avx2.hpp`, `fill_uniform01_avx2` (infra for later batches)
- `tests/cpp/simd_uniform_test.cpp`

### Batch 0 — Infrastructure

- Branch `v0.4.0`; CMake gate, cpuid, `results/baseline-v0.3.0/`

## Hand-written tracker (13)

| vault id | Tier B (v0.3.0) | Tier C SIMD | batch |
|----------|:---------------:|:-----------:|------:|
| `bernoulli` | SplitMix64 | **4-stream SplitMix** | 2 |
| `discrete-uniform` | SplitMix64 | **4-stream SplitMix** | 2 |
| `exponential` | SplitMix64 | **libmvec log1p** | 3 |
| `normal-gaussian` | SplitMix64 | **libmvec Box–Muller** | 3 |
| `geometric` | Tier A only | — | — |
| `binomial` | SplitMix64 | — | 4 |
| `negative-binomial` | SplitMix64 | — | 4 |
| `beta-binomial` | SplitMix64 | — | 4 |
| `poisson-binomial` | SplitMix64 | — | 4 |
| `categorical` | derived PCG | — | 5 |
| `zipf` | derived PCG | — | 5 |
| `zipfmandelbrot` | derived PCG | — | 5 |
| `skellam` | derived PCG | — | 5 |

## SIMD @10M vs v0.3.0 Tier B (cycles/sample)

| bench id | v0.3.0 | SIMD (batch 3) | speedup |
|----------|-------:|---------------:|--------:|
| bernoulli | 0.68 | 0.09 | **7.7×** |
| discrete-uniform | 0.86 | 0.47 | **1.8×** |
| exponential | 17.75 | 3.39 | **5.2×** |
| normal | 22.92 | 5.44 | **4.2×** |
| *(others unchanged — Tier B)* | | | ~1.0× |

Full CSVs: `results/baseline-v0.3.0/`, `results/current/` (SIMD build)

## Agent instructions

Read [`scripts/agent_simd_checklist.md`](scripts/agent_simd_checklist.md). One batch per session; push to `v0.4.0` after tests pass.

Phase 1 tracker: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md).
