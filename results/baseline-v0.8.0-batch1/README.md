# Table sampler polish (v0.8.0 batch 1)

Tier B @10M after binary search (continuous-bernoulli) and alias (discrete-weibull).

| dist | v0.7.0 Tier B | v0.8.0 | speedup |
|------|--------------:|-------:|--------:|
| continuous-bernoulli | 248.92 | 93.57 | **2.66×** |
| discrete-weibull | 46.60 | 11.84 | **3.94×** |

Gate: ≥1.2× vs prior Tier B — both pass.
