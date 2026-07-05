# Wave 3 baseline (v0.8.0 batch 0)

Tier-A codegen @10M for composition/transform candidates. Compare before promoting Tier B in batch 2.

Regenerate:

```bash
make build
.venv/bin/python bench/bench_codegen.py --wave3 --out results/baseline-v0.8.0-wave3
```

Compare vs `results/codegen-tier-a/` for speedup ratios.
