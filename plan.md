# Performance optimization plan

SIMD-friendly counter RNG + single-thread fast `sample_batch` for the hand-written core. **Multi-thread parallel is optional** — only if single-core gains are insufficient.

**This work was deferred** during the heuristic → family push (batches 1–11 on `v0.2.0`). The old ISPC bench path was never finished (`sample_ispc()` falls back to C++). This plan replaces that path — do **not** revive `ispc/kernels.ispc` as the primary strategy.

**Visible progress:** each batch commits and pushes to [`v0.3.0`](https://github.com/ckirua/distributions/tree/v0.3.0). Update [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md) after every batch. CI runs on push.

---

## Strategy: SIMD first, parallel optional

| Approach | When | Expected gain | Complexity |
|----------|------|---------------|------------|
| **Counter RNG + vector loops** (primary) | Uniform-heavy samplers, large `n` on **one core** | ~2–4× | Medium |
| **Algorithm swap** (Ziggurat, BTPE, …) | Transcendental- or branch-heavy samplers | ~2–10× | Medium–high |
| **Multi-thread chunks** (optional) | Huge offline batches, spare cores, still need more after SIMD | ~×cores | High (repro, OpenMP, chunk seeds) |

**Default path (batches 0–6):** single-thread fast paths only. No OpenMP, no `std::thread` in the hot library.

**Optional extension (batches 7–8):** add `batch_parallel.hpp` only if batch 6 benchmarks show insufficient throughput at `n=10M`.

---

## Goals

| Goal | Target |
|------|--------|
| Hand-written core (`MANUAL`, 13 vault ids) | Counter-RNG fast path where ROI is clear |
| Generated family samplers (175) | Unchanged in required path; optional parallel wrapper in batch 8 |
| Reproducibility | Documented seed contract (see below) |
| Bench regression | No >5% slowdown on serial path at `n=10_000` |
| Tests | `make test` + `make test-sanity` always green |

## Non-goals

- ISPC/codegen SIMD for all 189 distributions
- Multi-thread parallel as a requirement for v0.3.0
- Bit-identical output between serial PCG and fast counter-RNG paths
- Changing scipy sanity tolerances to hide statistical drift
- `-march=native` as default (keep optional via `DISTRIBUTIONS_ENABLE_NATIVE`)

---

## Skipped / owed work (context)

| Original intent | Current state | This plan |
|-----------------|---------------|-----------|
| Phase-1 ISPC kernels (7 ids) | Compiled, **never called** | Remove or gate in batch 0; replace with `detail/fast/` |
| SIMD batch sampling | None — scalar `for` + serial `Pcg32` | Batches 1–5 |
| Parallel `sample_batch` | None | **Optional** batches 7–8 only |
| Algorithm upgrades (Ziggurat, etc.) | Box–Muller normal, inversion binomial | Batches 3–5 |

Baseline throughput (serial C++, `n=10_000_000`, `-O3` — re-record in batch 0):

| vault id | ~ns/sample | Bottleneck |
|----------|----------:|------------|
| `bernoulli` | 1.2 | RNG + compare |
| `exponential` | 21 | `log1p` |
| `normal-gaussian` | 40 | Box–Muller transcendentals |
| `binomial` | varies | O(n) inversion loop |

---

## RNG & reproducibility contract

Two required tiers; parallel is a third **optional** tier:

| Tier | Engine | Repro vs serial PCG | Use when |
|------|--------|---------------------|----------|
| **A — Serial** | `Pcg32` (current) | Exact (reference) | `sample()`, small `n`, sanity tests |
| **B — Fast (SIMD)** | Counter RNG (Philox or xoshiro128+), 4–8 wide | Statistical match, not bit-identical | `sample_batch` when `n >= kFastThreshold` |
| **C — Parallel** _(optional)_ | Tier A or B per chunk, derived sub-seeds | Same distribution, different order | Only if batch 7+ enabled and `n` very large |

**Rules:**

- `sample(rng)` stays Tier A forever on hand-written types.
- `sample_batch` uses Tier B when `n >= kFastThreshold` (suggest `4096`) **if** a fast path exists for that distribution; else Tier A.
- Tier C is never the default; gate behind `DISTRIBUTIONS_ENABLE_PARALLEL` or explicit API if added.
- Document tiers in `include/distributions/README.md` and `DEVELOPMENT.md`.
- Batch 6 adds `tests/test_reproducibility.py`: Tier A vs Tier B → KS-compatible / same moments, not identical bytes.

---

## Architecture (target)

```
include/distributions/detail/
  rng.hpp              # Pcg32 (unchanged, Tier A)
  counter_rng.hpp      # NEW — Philox/xoshiro, 4-wide or 8-wide uniforms (Tier B)
  fast/
    uniform.hpp        # vector fill
    bernoulli.hpp
    exponential.hpp
    normal.hpp
    ...
  batch_parallel.hpp   # OPTIONAL (batch 7+) — OpenMP or std::thread chunking
```

Hand-written headers dispatch:

```cpp
if (n >= kFastThreshold)
    detail::fast::bernoulli_sample_batch(out, n, seed);  // Tier B, single-thread
else
    /* existing serial PCG loop */                       // Tier A
```

**Do not** add SIMD to `tools/codegen/recipes.py` per distribution in the required path.

---

## Required path — batches 0–6

### Batch 0 — Infrastructure & bench truth

**Branch:** create `v0.3.0` from `v0.2.0`.

| Task | Files |
|------|-------|
| Record baseline CSVs for 13 hand-written ids | `results/baseline-v0.2.0/` (tracked) |
| Add `make bench-core` target | `Makefile` |
| Gate dead ISPC: default OFF or remove stub | `CMakeLists.txt`, codegen dispatch template |
| Document RNG tiers A/B | `include/distributions/README.md`, `DEVELOPMENT.md` |
| Progress tracker | `OPTIMIZE_PROGRESS.md` |

**Acceptance:**

```bash
make codegen && make build && make test && make test-sanity
make bench-core
```

**Commit:** `Optimize batch 0: bench baseline + ISPC gate + RNG contract docs`

---

### Batch 1 — Counter RNG only (no parallel)

| Deliverable | Description |
|-------------|-------------|
| `detail/counter_rng.hpp` | 4× `double` in [0,1) per step; `seed(stream_id, counter)` |
| `detail/fast/uniform.hpp` | Fill buffer with counter RNG |
| Unit test | `tests/cpp/counter_rng_test.cpp` — stream independence, mean/var smoke |

**Explicitly out of scope:** `batch_parallel.hpp`, OpenMP, threads.

**Acceptance:** mean ≈ 0.5, var ≈ 1/12 over 1M draws. `make test` green.

**Commit:** `Optimize batch 1: counter RNG primitives`

---

### Batch 2 — Trivial discrete (3)

| vault id | Fast path | Speedup target @10M (1 core) |
|----------|-----------|------------------------------|
| `bernoulli` | vector uniforms + compare | ≥3× |
| `discrete-uniform` | vector int from uniforms | ≥3× |
| `geometric` | vector uniforms + per-lane transform | ≥2× |

---

### Batch 3 — Transform continuous (2)

| vault id | Fast path | Speedup target @10M (1 core) |
|----------|-----------|------------------------------|
| `exponential` | vector uniforms + `log1p` | ≥2× |
| `normal-gaussian` | Ziggurat or vectorized Box–Muller on counter pairs | ≥2× |

Sanity: `normal` and `exponential` must stay green in `make test-sanity`.

---

### Batch 4 — Counting discrete (4)

Focus on **better algorithms + counter RNG for inner uniforms**, single-thread.

| vault id | Fast path | Notes |
|----------|-----------|-------|
| `binomial` | BTPE or similar O(1) method | Keep Tier A for small trial count |
| `negative-binomial` | optimized rejection/inversion with counter batches | |
| `beta-binomial` | vectorized beta draw + binomial count | |
| `poisson-binomial` | batched inner Bernoulli trials | smoke only (no sanity) |

---

### Batch 5 — Heavy discrete (4)

| vault id | Fast path | Notes |
|----------|-----------|-------|
| `categorical` | batched uniforms + prefix search; SIMD if `k` small | |
| `zipf` | batched inversion with counter RNG | xfail sanity — don't regress smoke |
| `zipfmandelbrot` | same | no scipy sanity |
| `skellam` | batched Poisson difference | |

---

### Batch 6 — Integration & bench sign-off

| Task | Detail |
|------|--------|
| `kFastThreshold` constant | `detail/counter_rng.hpp` or `detail/fast/common.hpp`, default 4096 |
| Tier A fallback | small `n` always uses serial PCG |
| Repro tests | `tests/test_reproducibility.py` (Tier A vs B) |
| Bench compare | `python bench/compare_baseline.py` vs batch 0 |

**Decision gate:** if geomean speedup @10M on hand-written core ≥ target (e.g. 2×), **stop** — skip optional batches 7–8 unless user requests more.

**Per-batch acceptance (2–6):**

```bash
make build && make test && make test-sanity
make bench-core
python bench/compare_baseline.py   # no >5% regression on Tier A path
```

**Commits:** `Optimize batch N: fast sample_batch for <ids>`

---

## Optional extension — batches 7–8

Only proceed if batch 6 geomean speedup is insufficient **and** large multi-core batches are a real use case.

### Batch 7 — Parallel wrapper (optional)

| Deliverable | Description |
|-------------|-------------|
| `detail/batch_parallel.hpp` | `parallel_for(n, min_chunk, fn)` — OpenMP if available, else `std::thread` |
| Hand-written apply | Chunk Tier A or B over indices; sub-seed = `splitmix64(master, chunk_id)` |
| CMake option | `DISTRIBUTIONS_ENABLE_PARALLEL=OFF` by default |

Expect additional ~×cores on top of batch 6 gains for large `n`.

**Commit:** `Optimize batch 7 (optional): parallel sample_batch wrapper`

---

### Batch 8 — Family parallel + full bench (optional)

- Codegen `sample_batch` override: parallel chunks calling existing `sample()` (Tier C only).
- No per-recipe SIMD in `recipes.py`.
- `make bench-all` + update `results/README.md`.

**Commit:** `Optimize batch 8 (optional): parallel family sample_batch + bench sweep`

---

## Batch 9 — Cleanup

| Task | Action |
|------|--------|
| `ispc/` | Delete or move to `archive/ispc-phase1/` |
| `bench/sweep.py` | Remove `PHASE1_ISPC`; use `bench-core` |
| Docs | Document Tier A/B (and C if built); drop ISPC promises |

**Commit:** `Optimize batch 9: remove dead ISPC path, update docs`

---

## Metrics tracker

Update [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md) after each batch:

| Metric | Start (v0.2.0) | Current |
|--------|---------------:|--------:|
| Hand-written with Tier B fast path | 0 / 13 | |
| Hand-written with counter-RNG inner loop | 0 / 13 | |
| `bench-core` geomean speedup @10M (1 core) | 1.0× | |
| Parallel enabled (`batch_parallel.hpp`) | no | |
| Family with parallel wrapper | 0 / 175 | |
| ISPC stub removed | no | |

---

## Agent loop

See [`scripts/agent_optimize_checklist.md`](scripts/agent_optimize_checklist.md).

```
1. Read OPTIMIZE_PROGRESS.md → pick next batch
2. Implement batch scope only
3. make codegen && make build && make test && make test-sanity
4. make bench-core (+ compare_baseline when available)
5. Update OPTIMIZE_PROGRESS.md
6. git commit && git push origin v0.3.0
```

At batch 6: record geomean speedup; **skip batches 7–8** unless OPTIMIZE_PROGRESS marks them as requested.

---

## Risk register

| Risk | Mitigation |
|------|------------|
| Sanity failures after Ziggurat normal | Tier A path for sanity-sized runs; or documented xfail |
| Counter RNG quality concerns | Use Philox/xoshiro; statistical smoke in unit test |
| Scope creep into 175 family SIMD | Non-goal; optional batch 8 is parallel wrapper only |
| Parallel added too early | Not in batches 0–6; explicit decision gate at batch 6 |

---

## Summary batch list

| Batch | Required? | Scope |
|------:|:---------:|-------|
| 0 | yes | Baseline, ISPC gate, RNG docs |
| 1 | yes | `counter_rng.hpp` only |
| 2 | yes | bernoulli, discrete-uniform, geometric |
| 3 | yes | exponential, normal |
| 4 | yes | binomial, negative-binomial, beta-binomial, poisson-binomial |
| 5 | yes | categorical, zipf, zipfmandelbrot, skellam |
| 6 | yes | `kFastThreshold`, repro tests, bench sign-off |
| 7 | **optional** | `batch_parallel.hpp` + hand-written chunks |
| 8 | **optional** | parallel wrapper for 175 family samplers |
| 9 | yes | remove ISPC, doc cleanup |

**Required: 8 batches (0–6 + 9).** Optional parallel: batches 7–8, gated at batch 6.
