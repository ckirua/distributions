# v1.0.0 release baseline (v0.8.0 batch 3)

Frozen bench CSVs for all **9 promoted codegen Tier B** distributions @ 1k / 100k / 10M.

| Bench id | Tier-A (`tier-a/`) | Tier-B (`tier-b/`) | Speedup @10M |
|----------|-------------------|-------------------|-------------:|
| gamma | codegen-tier-a | v0.7.0 batch 2 | 2.94× |
| beta | codegen-tier-a | v0.7.0 batch 2 | 1.66× |
| continuous-bernoulli | codegen-tier-a | v0.7.0 batch 5 + v0.8.0 batch 1 | 35.43× |
| discrete-weibull | codegen-tier-a | v0.7.0 batch 5 + v0.8.0 batch 1 | 184.18× |
| erlang | codegen-tier-a | v0.7.0 batch 5 | 2.94× |
| log-normal | codegen-tier-a | v0.7.0 batch 5 | 1.63× |
| half-normal | codegen-tier-a | v0.8.0 batch 2 | 1.57× |
| inverse-gamma | codegen-tier-a | v0.8.0 batch 2 | 1.81× |
| beta-prime | codegen-tier-a | v0.8.0 batch 2 | 1.66× |

**Geomean speedup** (9 promoted @10M vs Tier-A): **4.47×**

(v0.7.0 sign-off geomean on 6 ids was **4.95×**; v1.0.0 adds 3 wave-3 ids with lower individual speedups, pulling the combined geomean down slightly.)

## Sign-off

- `make test-all`: **526 passed**, 11 xfailed, 3 xpassed
- Codegen Tier B: **9 / 171**
- Table samplers: binary search (continuous-bernoulli), alias (discrete-weibull)

Regenerate Tier-B numbers:

```bash
make build
.venv/bin/python bench/bench_codegen.py \
  --dist gamma --dist beta --dist continuous-bernoulli \
  --dist discrete-weibull --dist erlang --dist log-normal \
  --dist half-normal --dist inverse-gamma --dist beta-prime \
  --out results/baseline-v1.0.0/tier-b
```

Tier-A reference copies from `results/codegen-tier-a/`.
