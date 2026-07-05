# Heuristic sampler implementation progress

Goal: implement all **121** heuristic placeholders as real `family` samplers; all **189** pass `make test-sanity` (or documented xfail).

## Status (updated each batch)

| Metric | Count |
|--------|------:|
| Total distributions | 189 |
| `hand-written` | 13 |
| `family` | 61 |
| `heuristic` remaining | **114** |
| `exact` | 1 |
| Sanity tests passing | 38 (+ 12 xfail, 2 xpass) |

## Loop

- **Schedule:** `/loop 15m` — continue next category batch automatically
- **Prompt:** read this file → implement one category → `make codegen && make build && make test && make test-sanity` → update this file

## Completed batches

### Batch 1 — `continuous/semi-infinite-interval` (7 ids)

Implemented scipy-backed scalar samplers in `tools/codegen/recipes.py` + `SCIPY_SPECS`:

| vault id | scipy | notes |
|----------|-------|-------|
| `rice` | `rice` | `(b+Z1)²+Z2²` |
| `levy` | `levy` | `loc + scale/Z²` |
| `gompertz` | `gompertz` | inverse CDF |
| `truncated-normal` | `truncnorm` | rejection |
| `log-logistic` | `fisk` | inverse CDF |
| `log-laplace` | `loglaplace` | inverse CDF |
| `generalized-gamma` | `gengamma` | gamma power transform |

## Next batch (priority order)

1. **`directional/univariate-circular`** (5) — wrapped-normal, wrapped-cauchy, …
2. **`discrete/infinite-support`** (11) — borel, COM-Poisson, …
3. **`continuous/bounded-interval`** (8)
4. **`continuous/semi-infinite-interval`** (25 remaining)
5. **`continuous/whole-real-line`** (19)
6. **`multivariate/*`** (25) — may need vector API or scalar projection
7. **`continuous/variable-support`** (14)
8. **Other** (directional multivariate, degenerate, mixed)

## Remaining heuristic by category

Run to refresh:

```bash
.venv/bin/python - <<'PY'
import yaml
from collections import Counter
reg = yaml.safe_load(open(".vault/_meta/registry.yaml"))["distributions"]
for cat, n in sorted(Counter(e["category_path"] for e in reg if e.get("sampler_tier")=="heuristic").items()):
    print(f"{cat}: {n}")
PY
```
