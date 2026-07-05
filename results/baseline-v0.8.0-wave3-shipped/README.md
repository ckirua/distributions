# Wave 3 Tier B shipped (v0.8.0 batch 2)

Bench-gated @10M vs `results/codegen-tier-a/`. Gate: ≥ **1.5×**.

| dist | Tier-A | Tier-B | speedup | hook |
|------|-------:|-------:|--------:|------|
| half-normal | 37.25 | 23.76 | **1.57×** | normal + abs |
| inverse-gamma | 46.60 | 25.84 | **1.81×** | gamma + reciprocal |
| beta-prime | 97.40 | 58.75 | **1.66×** | dual gamma ratio |

## Skipped (below 1.5× or regression)

| dist | ratio | reason |
|------|------:|--------|
| weibull | 1.09× | transform-dominated; block uniform insufficient |
| rayleigh | 1.26× | near-miss |
| pareto | 0.99× | pow overhead |
| chi-squared | 0.93× | df=3 → non-integer gamma (Marsaglia) |
| chi | 0.97× | same |
| davis | 0.97× | same as chi-squared @ nu=3 |
| scaled-inverse-chi-squared | 0.94× | chi-squared bottleneck |
| student-t | 1.19× | near-miss (gamma batching improved but <1.5×) |

Fast path headers remain in `include/distributions/detail/fast/` for future promotion.
