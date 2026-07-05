# distributions

Statistical distributions for C++/Python — with an Obsidian learning vault in [`.vault/`](.vault/).

## Repository layout

```
include/distributions/   C++ samplers (by category — see include/distributions/README.md)
bench/                   Benchmark harness (run_bench, bench-core, sweep)
cydist/                  Cython Python bindings
tools/                   Library codegen (codegen_distributions.py)
.vault/                  Obsidian learning vault + vault build tools
tests/                   C++ unit tests + pytest
results/                 Benchmark CSV output
archive/                 Retired experiments (e.g. Phase-1 ISPC)
cmake/                   Toolchain helpers
```

Quick commands: `make help` · see [DEVELOPMENT.md](DEVELOPMENT.md) for full workflow

Usage examples: [`examples/`](examples/)

## Library

All **189** vault distributions have native C++ samplers (HPC-oriented: inverse/transform/rejection algorithms, no lookup tables). Headers are grouped by category under `include/distributions/`:

| Folder | Category |
|--------|----------|
| `discrete/finite/` | finite-support discrete |
| `discrete/infinite/` | infinite-support discrete |
| `continuous/bounded/` | bounded continuous |
| `continuous/semi_infinite/` | semi-infinite continuous |
| `continuous/real_line/` | whole real line |
| `continuous/variable/` | variable support |
| `multivariate/` | multivariate & matrix-valued |
| `directional/` | circular & spherical |
| `degenerate/` | degenerate / singular |
| `detail/` | shared fast primitives (gamma, normal, poisson, …) |

13 core samplers are hand-tuned with optional Tier-B fast paths; the rest are generated from [`tools/codegen_distributions.py`](tools/codegen_distributions.py).

See [`plan.md`](plan.md) and [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md) for the v0.3.0 optimization track.

### Build (C++)

Requires **GCC 14+** with **C++26** (`-std=c++26`). Ubuntu 24.04 ships `g++-14` by default.

```bash
make build          # configure + compile
make test           # ctest
./build/run_bench poisson 1000000 42
```

### Build (Python / Cython)

```bash
.venv/bin/pip install -r .vault/requirements.txt   # numpy, Cython, scipy, …
make install                                       # editable cydist (189 samplers)
.venv/bin/python -c "from cydist import __all__; print(len(__all__))"
```

### Benchmarks

```bash
make bench-core                         # 13 hand-written ids
make bench-all                          # all 189 distributions (C++ timings)
python bench/sweep.py --all --quick     # faster full-registry sweep
python bench/aggregate_summary.py       # → results/summary.csv
```

Regenerate C++ after editing sampler recipes: `make codegen`

Results: [`results/`](results/)

## Obsidian vault

Open [`.vault/`](.vault/) as an Obsidian vault. **189 distributions** from the [Wikipedia template](https://en.wikipedia.org/wiki/Template:Probability_distributions), with brief notes, charts, and wikilinks — no code in the learning sections.

Start at `.vault/00-Home/Index.md`.

Vault build tools: [`.vault/tools/README.md`](.vault/tools/README.md) · `make vault`
