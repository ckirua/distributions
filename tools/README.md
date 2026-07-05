# Library tools

Python utilities for the **C++/Python sampler library** (not the Obsidian vault — those live in [`.vault/tools/`](../.vault/tools/README.md)).

## Layout

```
tools/
  codegen_distributions.py   # entry point (thin wrapper)
  codegen/
    constants.py             # paths, MANUAL map, aliases
    models.py                # Recipe, CydistSpec
    utils.py                 # slug → class, safe param names
    recipes.py               # build_recipes() catalog
    emit.py                  # headers, dispatch, cydist, tests, registry tiers
    main.py                  # orchestration
```

## Regenerate all library outputs

```bash
make codegen
# or:
.venv/bin/python tools/codegen_distributions.py
```

| Output | Purpose |
|--------|---------|
| `include/distributions/{category}/*.hpp` | 176 generated samplers (13 hand-written preserved) |
| `include/distributions/all.hpp` | Umbrella includes |
| `bench/dispatch.hpp` | Benchmark router for all 189 vault ids |
| `cydist/cydist_shim.h`, `cydist_shim.cpp` | C ABI |
| `cydist/cydist.pyx`, `cydist.pxd` | Cython wrappers (**189** distributions) |
| `cydist/__init__.py`, `__init__.pyi` | Public exports + stubs |
| `tests/test_generated.py` | Smoke tests (189 C++ + 189 Python) |
| `.vault/_meta/registry.yaml` | `sampler_tier` field updated per distribution |

Edit sampler recipes in `tools/codegen/recipes.py`, then regenerate. Hand-tuned headers under `include/distributions/` listed in `MANUAL` are never overwritten.

## Sampler tiers

| Tier | Meaning |
|------|---------|
| `hand-written` | 13 core samplers (Bernoulli, Poisson, Normal, …) |
| `family` | Explicit algorithm from a known family (gamma, beta, Cauchy, …) |
| `heuristic` | Generic `loc + scale * N(0,1)` fallback |
| `exact` | Deterministic (e.g. Dirac delta) |

Tiers are written to `registry.yaml` on each `make codegen` run.
