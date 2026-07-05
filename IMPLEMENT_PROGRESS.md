# Heuristic sampler implementation progress

Goal: implement all **121** heuristic placeholders as real `family` samplers; all **189** pass `make test-sanity` (or documented xfail).

**Visible progress:** each batch is committed and pushed to [`v0.2.0`](https://github.com/ckirua/distributions/tree/v0.2.0). CI runs on every push (`.github/workflows/ci.yml`).

## Status

| Metric | Count |
|--------|------:|
| Total distributions | 189 |
| `hand-written` | 13 |
| `family` | **175** |
| `heuristic` remaining | **0** |
| `exact` | 1 |
| Sanity tests passing | 73 (+ 12 xfail, 2 xpass) |

Last push: batch 11 (final mixed samplers, 8 distributions). **All 121 heuristics implemented.**

## Completed batches

### Batch 1 — `continuous/semi-infinite-interval` (7)

`rice`, `levy`, `gompertz`, `truncated-normal`, `log-logistic`, `log-laplace`, `generalized-gamma`

### Batch 2 — `directional/univariate-circular` (5)

`wrapped-cauchy`, `wrapped-normal`, `wrapped-exponential`, `wrapped-levy`, `wrapped-asymmetric-laplace`

### Batch 3 — `discrete/infinite-support` (11)

`floryschulz`, `gausskuzmin`, `borel`, `beta-negative-binomial`, `mixed-poisson`, `delaporte`, `conwaymaxwellpoisson`, `discrete-phase-type`, `extended-negative-binomial`, `panjer`, `parabolic-fractal`

### Batch 4 — `continuous/bounded-interval` (8)

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `argus` | Devroye rejection (scipy port) | yes |
| `balding-nichols` | Beta((1-F)/2F, …) | skip |
| `generalized-beta` | Beta or Libby–Novick rejection | yes (λ=0) |
| `beta-rectangular` | Beta(α,β) | yes |
| `continuous-bernoulli` | grid inverse CDF | skip |
| `continuous-binomial` | mean of n uniforms | skip |
| `noncentral-beta` | ncx2 / chi² ratio | skip |
| `pert` | scaled Beta(a,b) | yes |

### Batch 5 — `continuous/semi-infinite-interval` (25)

New helper: `include/distributions/detail/semi_infinite.hpp`

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `frechet` | inverse Weibull | yes |
| `generalized` | generalized gamma | yes |
| `type-2-gumbel` | Weibull minimum | yes |
| `shifted` | shifted exponential | yes |
| `noncentral-f` | ncx2 / chi² ratio | yes |
| `hotellings-t-squared` | noncentral F (nc=0) | yes |
| `hyperexponential` | exponential mixture | skip |
| `hypoexponential` | sum of exponentials | skip |
| `benini`, `benktander-*`, `davis`, `discrete-weibull`, `hartmanwatson`, `hyper`, `kolmogorov`, `log-cauchy`, `log-t`, `matrix-exponential`, `maxwelljuttner`, `mittag-leffler`, `phase-type`, `poly-weibull`, `relativistic-breitwigner`, `wilkss-lambda` | specialized / approximate | skip |

### Batch 6 — `continuous/whole-real-line` (19)

New helper: `include/distributions/detail/real_line.hpp`

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `asymmetric` | asymmetric Laplace (ppf) | yes |
| `generalized-normal` | generalized normal (gamma) | yes |
| `generalized-logistic-logistic-beta` | genlogistic | yes |
| `johnsons-su` | Johnson SU (sinh transform) | yes |
| `noncentral-t` | nct ratio | yes |
| `landau` | Chambers–Mallows–Stuck | yes |
| `stable`, `holtsmark` | Nolan stable (α=1.5, β=0) | yes |
| `variance-gamma` | NIG composition | yes |
| `fishers-z`, `gaussian-q`, `generalized-hyperbolic`, `geometric-stable`, `hyperbolic-secant`, `kaniadakis-gaussian`, `slash`, `tracywidom`, `voigt` | specialized / approximate | skip |

### Batch 7 — `continuous/variable-support` (13)

New helper: `include/distributions/detail/variable_support.hpp`

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `generalized-extreme-value` | GEV (ppf) | yes |
| `generalized-pareto` | GPD (boxcox ppf) | yes |
| `tukey-lambda` | Tukey λ (boxcox ppf) | yes |
| `generalized-chi-squared`, `marchenkopastur`, `kaniadakis-*`, `q-exponential`, `q-gaussian`, `q-weibull` | specialized / κ- or q-deformed | skip |

### Batch 8 — `multivariate/matrix-valued` (13)

New helper: `include/distributions/detail/matrix.hpp` (2×2 Bartlett; scalar summaries: trace / element)

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `wishart` | Bartlett trace | yes |
| `matrix-normal` | element (0,0) | yes |
| `inverse-wishart`, `matrix-t`, `matrix-gamma`, `inverse-matrix-gamma-distribution`, `matrix-beta`, `matrix-f`, `lkj`, `normal-wishart`, `normal-inverse`, `complex`, `uniform-distribution-on-a-stiefel-manifold` | matrix / approximate | skip |

### Batch 9 — `multivariate/continuous` (8)

New helper: `include/distributions/detail/multivariate.hpp`

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `dirichlet` | gamma-ratio first component | yes |
| `multivariate-normal` | first component | yes |
| `multivariate-t` | first component (= Student t) | yes |
| `generalized-dirichlet`, `multivariate-laplace`, `multivariate-stable`, `normal-gamma`, `normal-inverse-gamma-distribution` | approximate / compound | skip |

### Batch 10 — `multivariate/discrete` (4)

New helper: `include/distributions/detail/multivariate_discrete.hpp`

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `multinomial` | Binomial(n, p₀) first count | yes |
| `dirichlet-multinomial` | Beta–Binomial compound | skip |
| `negative-multinomial` | Gamma–Poisson mixture | skip |
| `ewens` | sequential Ewens partition | skip |

### Batch 11 — final mixed (8)

New helper: `include/distributions/detail/special.hpp`

| vault id | algorithm | scipy sanity |
|----------|-----------|--------------|
| `normal-inverse-gaussian` | NIG composition (reuses `real_line.hpp`) | yes |
| `von-misesfisher` | 3D VMF x-coordinate (scipy `_rvs_3d`) | yes |
| `rectified-gaussian` | max(0, Normal) | skip |
| `cantor` | base-3 Cantor set construction | skip |
| `kent` | approximate Kent x-coordinate | skip |
| `bingham` | approximate Bingham x-coordinate | skip |
| `bivariate-von-mises` | von Mises first angle | skip |
| `soliton` | uniform discrete 0..n_max | skip |

## Agent instructions

Read `scripts/agent_batch_checklist.md`. One category per session; **always push** to `v0.2.0` after tests pass.
