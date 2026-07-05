# Heuristic sampler implementation progress

Goal: implement all **121** heuristic placeholders as real `family` samplers; all **189** pass `make test-sanity` (or documented xfail).

**Visible progress:** each batch is committed and pushed to [`v0.2.0`](https://github.com/ckirua/distributions/tree/v0.2.0). CI runs on every push (`.github/workflows/ci.yml`).

## Status

| Metric | Count |
|--------|------:|
| Total distributions | 189 |
| `hand-written` | 13 |
| `family` | **110** |
| `heuristic` remaining | **65** |
| `exact` | 1 |
| Sanity tests passing | 53 (+ 12 xfail, 2 xpass) |

Last push: batch 5 (semi-infinite continuous, 25 samplers).

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

## Next batch

**`continuous/whole-real-line`** (19 heuristic): `asymmetric-laplace`, `cauchy` done, `exponential-power`, …

## Agent instructions

Read `scripts/agent_batch_checklist.md`. One category per session; **always push** to `v0.2.0` after tests pass.
