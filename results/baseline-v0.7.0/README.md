# v0.7.0 Tier-B sign-off (batch 6)

Frozen bench CSVs for the **6 promoted codegen** distributions @ 1k / 100k / 10M.

| Bench id | Tier-A (`*-tier-a.csv`) | Tier-B (`*-tier-b.csv`) | Speedup @10M |
|----------|-------------------------|-------------------------|-------------:|
| gamma | `codegen-tier-a/` | v0.7.0 batch 2 | 2.97× |
| beta | `codegen-tier-a/` | v0.7.0 batch 2 | 1.67× |
| continuous-bernoulli | `codegen-tier-a/` | v0.7.0 batch 5 | 13.38× |
| discrete-weibull | `codegen-tier-a/` | v0.7.0 batch 5 | 46.78× |
| erlang | `codegen-tier-a/` | v0.7.0 batch 5 | 2.96× |
| log-normal | `codegen-tier-a/` | v0.7.0 batch 5 | 1.60× |

**Geomean speedup** (6 promoted @10M vs Tier-A): **4.95×**

Hand-written `bench-core` vs `baseline-v0.2.0` @10M: **1.315×** geomean (no regression >5%).

Regenerate Tier-B numbers:

```bash
make build
.venv/bin/python bench/bench_codegen.py \
  --dist gamma --dist beta --dist continuous-bernoulli \
  --dist discrete-weibull --dist erlang --dist log-normal \
  --out results/current
```
