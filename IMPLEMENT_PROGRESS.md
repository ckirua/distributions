# Heuristic sampler implementation progress

Goal: implement all **121** heuristic placeholders as real `family` samplers; all **189** pass `make test-sanity` (or documented xfail).

**Visible progress:** each batch is committed and pushed to [`v0.2.0`](https://github.com/ckirua/distributions/tree/v0.2.0). CI runs on every push (`.github/workflows/ci.yml`).

## Status

| Metric | Count |
|--------|------:|
| Total distributions | 189 |
| `hand-written` | 13 |
| `family` | **66** |
| `heuristic` remaining | **109** |
| `exact` | 1 |
| Sanity tests passing | 39 (+ 12 xfail, 2 xpass) |

Last push: batch 2 (wrapped circular, 5 samplers).

## Completed batches

### Batch 1 — `continuous/semi-infinite-interval` (7)

| vault id | scipy |
|----------|-------|
| `rice`, `levy`, `gompertz`, `truncated-normal`, `log-logistic`, `log-laplace`, `generalized-gamma` | yes |

### Batch 2 — `directional/univariate-circular` (5)

| vault id | scipy | notes |
|----------|-------|-------|
| `wrapped-cauchy` | `wrapcauchy` | scipy ppf port in `detail/circular.hpp` |
| `wrapped-normal` | — | wrap Gaussian |
| `wrapped-exponential` | — | wrap Exp |
| `wrapped-levy` | — | wrap Levy |
| `wrapped-asymmetric-laplace` | — | wrap Laplace |

## Next batch

**`discrete/infinite-support`** (11): `borel`, `conwaymaxwellpoisson`, `delaporte`, …

## Agent instructions

Read `scripts/agent_batch_checklist.md`. One category per session; **always push** to `v0.2.0` after tests pass.

Do **not** rely on bash `/loop` alone — it does not commit, push, or run the agent without an active chat.
