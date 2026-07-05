# Development guide

End-to-end workflow for the **distributions** library (C++, Python, benchmarks). The Obsidian vault in `.vault/` is separate — see [`.vault/tools/README.md`](.vault/tools/README.md).

## Prerequisites

- **GCC 14+** with C++26 (`g++-14`)
- **CMake** 3.20+
- **Python 3.10+** with venv

```bash
python3 -m venv .venv
.venv/bin/pip install -e ".[dev]"
.venv/bin/pip install -r .vault/requirements.txt   # vault tools (optional)
```

## Daily loop

```bash
make codegen      # regenerate headers, cydist, dispatch, tests, registry tiers
make build        # CMake: run_bench, sample_dump, tests, examples
make install      # editable cydist Python extension
make test         # ctest + fast pytest smoke (~379 tests)
make test-sanity  # statistical vs scipy (~48 tier-A samplers, slow)
make test-all     # smoke + sanity
make bench-all    # benchmark all 189 distributions → results/summary.csv
```

One-shot check:

```bash
make codegen build install test bench-all
```

## Project layout

| Path | Role |
|------|------|
| `include/distributions/` | C++ samplers by category + `detail/` primitives |
| `cydist/` | Python/Cython bindings (mostly generated) |
| `bench/` | `run_bench`, `sweep.py`, `dispatch.hpp` |
| `tools/codegen/` | Codegen implementation |
| `tests/` | C++ unit tests + generated Python smoke tests |
| `results/` | Benchmark CSVs (`summary.csv` tracked; per-dist CSVs gitignored) |
| `.vault/_meta/registry.yaml` | Single source of truth for all 189 distributions |

## Codegen

Entry point: `tools/codegen_distributions.py` → `tools/codegen/main.py`.

After editing recipes in `tools/codegen/recipes.py`:

```bash
make codegen
make build install test
```

**Never edit by hand:** `all.hpp`, `bench/dispatch.hpp`, `cydist/cydist.pyx`, `cydist_shim.*`, `cydist.pxd`, `tests/test_generated.py`.

## Tests

| Suite | Command | What it checks |
|-------|---------|----------------|
| C++ unit | `ctest --test-dir build` | Bernoulli, counter RNG, all-189 dispatch smoke |
| Python smoke | `make test` | Each `cydist.*_sample_batch` runs without error (189×2 + export count) |
| **Sanity** | `make test-sanity` | Tier-A samplers (`hand-written` + `family`) vs `scipy.stats` — moments + KS/chi-square (~31 pass, ~12 xfail) |
| Full | `make test-all` | Smoke + sanity |

Sanity tests sample `N=100_000` from **cydist** and **scipy** independently, then compare using helpers in [`tests/statistical_compare.py`](tests/statistical_compare.py) (same logic as [`bench/verify.py`](bench/verify.py)).

Coverage rules ([`tests/scipy_reference.py`](tests/scipy_reference.py)):

| Tier | Sanity tests |
|------|----------------|
| `hand-written`, `family` | Yes, when an exact `SCIPY_SPECS` entry exists (~43 cases: 31 pass, 12 xfail) |
| `heuristic` (121) | No — smoke only |
| `exact` (Dirac) | No — trivial constant |

Skipped (no scipy reference): `poisson-binomial`, `zipfmandelbrot`, and 11 family ids without `SCIPY_SPECS`.

Expected failures (`xfail`): approximate or mismatched samplers including `univariate-von-mises`, `circular-uniform`, `hypergeometric`, `zipf`, `logistic`, `cauchy`, and several bounded/semi-infinite family recipes pending algorithm fixes.

## Benchmarks

```bash
make bench-core            # 13 hand-written ids → results/current/
make bench-core-quick
make bench-all             # all 189 distributions → results/summary.csv
python bench/compare_baseline.py --geomean              # sign-off @ 10M (max 5% regression)
python bench/compare_baseline.py --n 1000 --geomean   # noisy Tier-A spot check
```

Frozen Tier-A baseline CSVs live in `results/baseline-v0.2.0/` (v0.2.0 serial path). Current runs overwrite `results/current/` only.

Phase-1 ISPC kernels were removed in optimize batch 9; see `archive/ispc-phase1/`.

### RNG tiers (hand-written core)

| Tier | When | Engine | Reproducibility |
|------|------|--------|-----------------|
| **A** | `sample()`, or `sample_batch` with `n < kFastThreshold` (4096) | Serial `Pcg32` | Bit-exact stream |
| **B** | `sample_batch` with `n >= 4096` when a fast path exists | SplitMix64 or derived-seed PCG | Statistical match only |
| **C** | Same as B when `-DDISTRIBUTIONS_ENABLE_SIMD=ON` and CPU has AVX2 | AVX2 (`detail/simd/`) | Statistical match only |
| **C512** | Same as B when `-DDISTRIBUTIONS_ENABLE_AVX512=ON` (requires SIMD) and CPU has AVX-512F | 8-wide double kernels (`detail/simd/avx512/`) | Statistical match only |

Build SIMD (optional, off by default):

```bash
make build-simd       # separate build dir: build-simd/
make build-simd512    # AVX-512 spike (Intel HPC): build-simd512/
```

Phase 2 tracker: [`plan-simd.md`](plan-simd.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md). Baseline for SIMD comparisons: `results/baseline-v0.3.0/` (Tier B @ v0.3.0).

Phase 3 (typed API + float): [`plan-api.md`](plan-api.md), [`API_PROGRESS.md`](API_PROGRESS.md) on branch `v0.5.0`.

Phase 4 (parameter validation): [`plan-validation.md`](plan-validation.md), [`VALIDATION_PROGRESS.md`](VALIDATION_PROGRESS.md) on branch `v0.6.0`. C++ ctors use `detail/validate.hpp` asserts (compiled out in `-DNDEBUG` / Release).

`geometric` has no Tier-B path (SplitMix regressed scipy variance). See [`tests/test_reproducibility.py`](tests/test_reproducibility.py) for Tier A vs B checks on the other 12 hand-written ids.

```bash
make bench                    # alias for bench-core (13 hand-written ids)
python bench/sweep.py --all   # full 1k / 100k / 10M sweep (189 ids, C++ only)
python bench/aggregate_summary.py
```

Results land in `results/`. Tracked: `summary.csv`, `README.md`, `baseline-v0.2.0/` (Tier-A hand-written baseline).

## Vault (optional)

Rebuild Obsidian notes from Wikipedia template:

```bash
make vault
```

Vault tools live only in `.vault/tools/` — not in root `tools/`.

## Examples

```bash
make examples-cpp      # C++ header + C ABI demos
make examples-python   # cydist NumPy buffers
make examples-cython   # cimport shim with nogil
```

See [`examples/README.md`](examples/README.md).
