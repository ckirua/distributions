# C++ distribution samplers

Headers are grouped by vault category. Shared fast primitives live in [`detail/`](detail/).

| Directory | Vault category |
|-----------|----------------|
| `discrete/finite/` | finite-support discrete |
| `discrete/infinite/` | infinite-support discrete |
| `continuous/bounded/` | bounded continuous |
| `continuous/semi_infinite/` | semi-infinite continuous |
| `continuous/real_line/` | whole real line |
| `continuous/variable/` | variable support |
| `multivariate/` | multivariate & matrix-valued |
| `directional/` | circular & spherical |
| `degenerate/` | degenerate / singular |
| `mixed/` | mixed continuous–discrete |

**Hand-written** (13): Bernoulli, binomial, categorical, discrete uniform, Zipf, beta-binomial, Poisson-binomial, geometric, negative binomial, Skellam, exponential, normal.

**Generated** (176): run `make codegen` — see [`tools/README.md`](../../tools/README.md).

Root files: `rng.hpp`, `base.hpp`, `all.hpp` (generated umbrella).

## RNG tiers (optimization)

| Tier | Engine | Use |
|------|--------|-----|
| **A — Serial** | `Pcg32` in [`rng.hpp`](rng.hpp) | `sample()`, small batches, tests — bit-exact stream |
| **B — Fast** | SplitMix64 or derived-seed PCG (`detail/fast/`) | Large `sample_batch` when a fast path exists |

**Threshold:** `detail::kFastThreshold` is **4096** (see [`detail/counter_rng.hpp`](detail/counter_rng.hpp)). When `n >= kFastThreshold` and a Tier-B implementation exists, `sample_batch` dispatches to the fast path; otherwise the serial PCG loop runs.

Tier B is statistically equivalent, not bit-identical to Tier A. Repro tests: [`tests/test_reproducibility.py`](../../tests/test_reproducibility.py). See [`plan.md`](../../plan.md).
