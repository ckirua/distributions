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
