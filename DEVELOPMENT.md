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
| C++ unit | `ctest --test-dir build` | Bernoulli moments + all-189 dispatch smoke |
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
make bench                    # Phase-1 ISPC candidates (7 ids)
make bench-all                # all 189, quick batch sizes
python bench/sweep.py --all   # full 1k / 100k / 10M sweep
python bench/aggregate_summary.py
```

Results land in `results/`. Only `summary.csv` and `README.md` are tracked in git.

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
