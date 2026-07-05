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
make test         # ctest + pytest (379 tests)
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
| C++ unit | `ctest --test-dir build` | Bernoulli correctness + all-189 smoke via `dispatch.hpp` |
| Python smoke | `pytest tests/` | Each `cydist.*_sample_batch` runs without error |
| C++ bench subprocess | (in pytest) | `run_bench <id> cpp 1000 42` for every registry id |

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
