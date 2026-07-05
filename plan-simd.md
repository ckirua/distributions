# SIMD optimization plan (Phase 2)

**Prerequisite:** v0.3.0 scalar Tier-B fast paths complete (geomean **1.29×** @10M). This plan adds **hardware SIMD** (AVX2 first) as **Tier C** on top of existing Tier A/B — it does not replace them.

**Branch:** `v0.4.0` from `v0.3.0`.

**Visible progress:** each batch commits and pushes to [`v0.4.0`](https://github.com/ckirua/distributions/tree/v0.4.0). Update [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md) after every batch. CI runs on push.

**Do not** revive `archive/ispc-phase1/` as the primary strategy. New code lives under `include/distributions/detail/simd/`.

---

## What v0.3.0 actually shipped vs this plan

| v0.3.0 (Tier B) | Phase 2 (Tier C) |
|-----------------|------------------|
| Scalar SplitMix64 / derived PCG | AVX2 (optional AVX-512) intrinsics |
| Scalar `for` loops | 4-wide or 8-wide lanes per step |
| ~1.29× geomean | Target **≥2×** geomean vs v0.2.0 baseline |
| Philox counter RNG (infra only) | Philox or xoshiro **vectorized** uniform generation |

Compiler flags (`-O3`, `-march=native`) alone are **out of scope** — they are not a batch deliverable.

---

## Three-tier dispatch (updated)

| Tier | Engine | When | Repro vs Tier A |
|------|--------|------|-----------------|
| **A — Serial** | `Pcg32` | `sample()`, `n < kFastThreshold` | Bit-exact |
| **B — Fast scalar** | SplitMix64 / derived PCG (`detail/fast/`) | `n >= kFastThreshold`, SIMD off or no kernel | Statistical |
| **C — SIMD** | AVX2 counter-RNG + vector ops (`detail/simd/`) | `n >= kFastThreshold` **and** `DISTRIBUTIONS_ENABLE_SIMD=ON` **and** CPU supports AVX2 | Statistical |

**Rules:**

- Tier B remains the fallback when SIMD is disabled or unsupported (CI builds AVX2-off by default).
- `sample(rng)` stays Tier A forever on hand-written types.
- `tests/test_reproducibility.py` gains Tier B vs Tier C cases (not bit-identical).
- Do **not** add per-recipe SIMD in `tools/codegen/recipes.py` in the required path.

---

## Strategy

| Approach | ROI | Batches |
|----------|-----|---------|
| **Vector uniforms + compare** | Highest (bernoulli, discrete-uniform) | 2 |
| **Vector transforms** (`log`, `sqrt`, sin/cos) | Medium (exponential, normal) | 3 |
| **Vector inner Bernoulli trials** | Medium (binomial, poisson-binomial) | 4 |
| **Search / alias / CDF** | Low (categorical, zipf, skellam) | 5 — best-effort or skip |
| **Multi-thread chunks** | Optional if SIMD geomean < 2× | 7–8 (reuse old plan) |

**Implementation choice:** hand-written **AVX2 intrinsics** (`<immintrin.h>`) in `detail/simd/`, not ISPC. Optional later batch may wrap hot helpers with **xsimd** if maintenance cost grows.

**Math library:** prefer **`__log1p_pd` / `_mm256_log_pd`** via `-lmvec` (glibc vector math) where available; document `-fno-math-errno` + statistical tests. Avoid `-ffast-math` by default.

---

## Architecture (target)

```
include/distributions/detail/
  counter_rng.hpp       # scalar Philox (unchanged)
  fast/                 # Tier B scalar (unchanged)
  simd/
    config.hpp          # DISTRIBUTIONS_HAS_AVX2, lane width
    philox4x32_avx2.hpp # 4× u32 → 4× double [0,1) per step
    uniform.hpp         # fill_uniform01_avx2
    bernoulli.hpp
    discrete_uniform.hpp
    exponential.hpp
    normal.hpp
    binomial.hpp
    ...
```

Hand-written dispatch pattern:

```cpp
if (n >= detail::kFastThreshold) {
#if defined(DISTRIBUTIONS_HAS_AVX2)
    detail::simd::bernoulli_sample_batch(...);  // Tier C
    return;
#endif
    detail::fast::bernoulli_sample_batch(...);   // Tier B fallback
    return;
}
```

---

## Batches

### Batch 0 — Infrastructure & SIMD baseline

| Task | Detail |
|------|--------|
| Branch `v0.4.0` | from `v0.3.0` |
| CMake | `DISTRIBUTIONS_ENABLE_SIMD=OFF` default; `-DDISTRIBUTIONS_ENABLE_SIMD=ON` adds `-mavx2 -mfma` and `DISTRIBUTIONS_HAS_AVX2=1` |
| Runtime guard | `simd::cpu_has_avx2()` — cpuid check; scalar Tier B if false |
| Baseline CSVs | `results/baseline-v0.3.0/` — Tier-B @10M (new frozen reference) |
| Progress tracker | `SIMD_PROGRESS.md`, `scripts/agent_simd_checklist.md` |
| Docs | Tier A/B/C in README + DEVELOPMENT |

**Acceptance:** `make codegen && make build && make test && make test-sanity` (SIMD **off**). `make bench-core` writes baseline.

**Commit:** `SIMD batch 0: AVX2 gate + v0.3.0 baseline`

---

### Batch 1 — Vector counter-RNG + uniforms

| Deliverable | Detail |
|-------------|--------|
| `detail/simd/philox4x32_avx2.hpp` | 4-lane Philox4x32-10; counter step produces `__m256d` uniforms |
| `detail/simd/uniform.hpp` | `fill_uniform01_avx2(double* out, size_t n, seed)` |
| Unit test | `tests/cpp/simd_uniform_test.cpp` — mean ≈ 0.5, var ≈ 1/12; KS vs scalar Philox |

**Out of scope:** distribution samplers, AVX-512.

**Acceptance:** ctest green; SIMD build optional in CI matrix (one job ON).

**Commit:** `SIMD batch 1: AVX2 Philox uniform fill`

---

### Batch 2 — Trivial discrete (2–3)

| vault id | Vector approach | Speedup target @10M vs v0.3.0 Tier B |
|----------|-----------------|-------------------------------------:|
| `bernoulli` | 4× uniform → compare mask → pack int | ≥2× |
| `discrete-uniform` | 4× uniform → mul+add → truncate to int | ≥2× |
| `geometric` | optional — only if scipy sanity passes | ≥1.5× |

Wire Tier C in hand-written headers; Tier B unchanged as `#else` fallback.

**Commit:** `SIMD batch 2: AVX2 bernoulli + discrete-uniform`

---

### Batch 3 — Transform continuous (2)

| vault id | Vector approach | Notes |
|----------|-----------------|-------|
| `exponential` | 4× `log1p` on uniform block (libmvec or scalar fallback per lane) | |
| `normal-gaussian` | 4× Box–Muller (2 pairs/lane block) or revisit Ziggurat with fixed tables | sanity must stay green |

**Commit:** `SIMD batch 3: AVX2 exponential + normal`

---

### Batch 4 — Counting discrete (4)

| vault id | Vector approach |
|----------|-----------------|
| `binomial` | SIMD Bernoulli trial sums (`n ≤ 256`) |
| `beta-binomial` | vector beta draw (scalar?) + SIMD binomial count |
| `negative-binomial` | harder — may stay Tier B only; document |
| `poisson-binomial` | SIMD trial compares per probability slot |

**Commit:** `SIMD batch 4: AVX2 counting discrete`

---

### Batch 5 — Heavy discrete (4)

| vault id | Expected outcome |
|----------|------------------|
| `categorical` | alias: vector uniforms help RNG half only — modest gain |
| `zipf` / `zipfmandelbrot` | CDF search stays scalar — **skip Tier C** if bench neutral |
| `skellam` | dual Poisson — partial vectorization of inner Poisson |

**Commit:** `SIMD batch 5: heavy discrete (best-effort)`

---

### Batch 6 — Integration & sign-off

| Task | Detail |
|------|--------|
| Repro tests | `tests/test_reproducibility_simd.py` — Tier B vs Tier C |
| Bench compare | `python bench/compare_baseline.py --baseline results/baseline-v0.3.0 --geomean` |
| Decision gate | geomean vs v0.2.0 baseline ≥ **2×**? Skip batch 7 if yes |

**Commit:** `SIMD batch 6: integration + bench sign-off`

---

### Batch 7 — Optional parallel wrapper

Only if batch 6 geomean < 2× **and** multi-core batches matter.

Reuse [`plan.md`](plan.md) batches 7–8 design: `batch_parallel.hpp`, chunk Tier B or C per core.

**Commit:** `SIMD batch 7 (optional): parallel sample_batch`

---

## Per-batch acceptance (1–6)

```bash
make codegen && make build && make test && make test-sanity
# SIMD-off build always green

cmake -DDISTRIBUTIONS_ENABLE_SIMD=ON .. && make build
make bench-core OUT=results/current
python bench/compare_baseline.py --baseline results/baseline-v0.3.0 --geomean
```

Record speedup vs **v0.3.0 Tier B**, not v0.2.0, in `SIMD_PROGRESS.md`.

---

## Metrics tracker

Update [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md):

| Metric | Start (v0.3.0) | Current |
|--------|---------------:|--------:|
| Hand-written with Tier C SIMD path | 0 / 13 | |
| AVX2 enabled in default CI | no | |
| `bench-core` geomean vs v0.3.0 @10M | 1.0× | |
| `bench-core` geomean vs v0.2.0 @10M | 1.29× | |

---

## Risk register

| Risk | Mitigation |
|------|------------|
| CI runners without AVX2 | Runtime cpuid guard; SIMD-off default build |
| libmvec availability | Per-lane scalar fallback in simd helpers |
| Normal Ziggurat tables bad (batch 3 v0.3.0) | Stick to vector Box–Muller first |
| Geometric sanity failure (SplitMix v0.3.0) | Tier C only with Philox + extra sanity |
| Maintenance of intrinsics | Centralize in `detail/simd/`; max 8 files |

---

## Agent loop

See [`scripts/agent_simd_checklist.md`](scripts/agent_simd_checklist.md).

```
1. Read SIMD_PROGRESS.md → pick next batch
2. Implement batch scope only
3. make test && make test-sanity (SIMD off)
4. Build with -DDISTRIBUTIONS_ENABLE_SIMD=ON; bench-core; compare vs v0.3.0 baseline
5. Update SIMD_PROGRESS.md
6. git commit && git push origin v0.4.0
```

---

## Summary batch list

| Batch | Required? | Scope |
|------:|:---------:|-------|
| 0 | yes | AVX2 CMake gate, cpuid, v0.3.0 baseline |
| 1 | yes | vector Philox + uniform fill |
| 2 | yes | bernoulli, discrete-uniform |
| 3 | yes | exponential, normal |
| 4 | yes | binomial, beta-binomial, neg-bin, poisson-binomial |
| 5 | yes | categorical, zipf, zipfmandelbrot, skellam (best-effort) |
| 6 | yes | repro tests, bench sign-off |
| 7 | optional | parallel wrapper |

**Required: 7 batches (0–6).** Optional parallel: batch 7.
