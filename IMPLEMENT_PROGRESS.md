# Heuristic sampler implementation progress

Goal: implement all **121** heuristic placeholders as real `family` samplers; all **189** pass `make test-sanity` (or documented xfail).

**Visible progress:** each batch is committed and pushed to [`v0.2.0`](https://github.com/ckirua/distributions/tree/v0.2.0). CI runs on every push (`.github/workflows/ci.yml`).

## Status

| Metric | Count |
|--------|------:|
| Total distributions | 189 |
| `hand-written` | 13 |
| `family` | **77** |
| `heuristic` remaining | **98** |
| `exact` | 1 |
| Sanity tests passing | 43 (+ 12 xfail, 2 xpass) |

Last push: batch 3 (discrete infinite, 11 samplers).

## Completed batches

### Batch 1 — `continuous/semi-infinite-interval` (7)

| vault id | scipy |
|----------|-------|
| `rice`, `levy`, `gompertz`, `truncated-normal`, `log-logistic`, `log-laplace`, `generalized-gamma` | yes |

### Batch 2 — `directional/univariate-circular` (5)

| vault id | scipy | notes |
|----------|-------|-------|
| `wrapped-cauchy` | `wrapcauchy` | scipy ppf port in `detail/circular.hpp` |
| `wrapped-normal`, `wrapped-exponential`, `wrapped-levy`, `wrapped-asymmetric-laplace` | — | wrap base sampler |

### Batch 3 — `discrete/infinite-support` (11)

| vault id | algorithm |
|----------|-----------|
| `floryschulz` | P(1)=p; else 2+NB(2,p) |
| `gausskuzmin` | inverse CDF (continued-fraction pmf) |
| `borel` | inverse CDF |
| `beta-negative-binomial` | Beta × NB compound |
| `mixed-poisson` | Gamma × Poisson |
| `delaporte` | Poisson + NB |
| `conwaymaxwellpoisson` | COM-Poisson (ν=1 → Poisson) |
| `discrete-phase-type` | sum of two geometric phases |
| `extended-negative-binomial` | gamma–Poisson NB |
| `panjer` | Poisson (a,b,0) member |
| `parabolic-fractal` | inverse CDF n^{-b} exp(-c log² n) |

Sanity vs scipy: `panjer`, `conwaymaxwellpoisson`, `extended-negative-binomial`, `mixed-poisson`.

## Next batch

**`continuous/bounded-interval`** (8): `argus`, `beta-rectangular`, `kumaraswamy`, …

## Agent instructions

Read `scripts/agent_batch_checklist.md`. One category per session; **always push** to `v0.2.0` after tests pass.
