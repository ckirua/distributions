# Implementation index

> [!summary]
> Maps future repository symbols to vault learning notes. Status values: `none` → `planned` → `benchmarked` → `implemented`.

## Sync convention

1. Source of truth for vault metadata: [`_meta/registry.yaml`](../_meta/registry.yaml)
2. When `dist.hpp` / `cydist` adds a sampler, set `code_status: implemented` on that distribution's frontmatter (or update registry and rebuild).
3. Re-run `.venv/bin/python .vault/tools/build_vault.py` to refresh this table and MOCs.
4. Benchmark results stay in repo `results/` — not duplicated here.

## Phase 1 planned samplers

| Distribution | C++ symbol | Python symbol | Status |
|--------------|------------|---------------|--------|
| [[bernoulli]] | `distributions::Bernoulli` | `cydist.bernoulli` | planned |
| [[beta-binomial]] | `distributions::BetaBinomial` | `cydist.beta_binomial` | planned |
| [[binomial]] | `distributions::Binomial` | `cydist.binomial` | planned |
| [[categorical]] | `distributions::Categorical` | `cydist.categorical` | planned |
| [[discrete-uniform]] | `distributions::DiscreteUniform` | `cydist.discrete_uniform` | planned |
| [[exponential]] | `distributions::Exponential` | `cydist.exponential` | planned |
| [[geometric]] | `distributions::Geometric` | `cydist.geometric` | planned |
| [[negative-binomial]] | `distributions::NegativeBinomial` | `cydist.negative_binomial` | planned |
| [[normal-gaussian]] | `distributions::Normal` | `cydist.normal` | planned |
| [[poisson-binomial]] | `distributions::PoissonBinomial` | `cydist.poisson_binomial` | planned |
| [[skellam]] | `distributions::Skellam` | `cydist.skellam` | planned |
| [[zipf]] | `distributions::Zipf` | `cydist.zipf` | planned |
| [[zipfmandelbrot]] | `distributions::ZipfMandelbrot` | `cydist.zipf_mandelbrot` | planned |

## Catalog coverage

- **189** distribution notes in vault
- **13** with planned code symbols
- **176** learning-only (no current implementation plan)

See [[Sampling vs density]] for how notes relate to future `sample()` / `sample_batch()` functions.
