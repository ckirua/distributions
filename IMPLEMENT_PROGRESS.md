# Heuristic sampler implementation progress

Goal: implement all **121** heuristic placeholders as real `family` samplers; all **189** pass `make test-sanity` (or documented xfail).

**Visible progress:** each batch is committed and pushed to [`v0.2.0`](https://github.com/ckirua/distributions/tree/v0.2.0). CI runs on every push (`.github/workflows/ci.yml`).

## Status

| Metric | Count |
|--------|------:|
| Total distributions | 189 |
| `hand-written` | 13 |
| `family` | **85** |
| `heuristic` remaining | **90** |
| `exact` | 1 |
| Sanity tests passing | 47 (+ 12 xfail, 2 xpass) |

Last push: batch 4 (bounded continuous, 8 samplers).

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

## Next batch

**`continuous/semi-infinite-interval`** (25 remaining): `benini`, `gompertz` done, `frechet`, `generalized-gamma` done, …

## Agent instructions

Read `scripts/agent_batch_checklist.md`. One category per session; **always push** to `v0.2.0` after tests pass.
