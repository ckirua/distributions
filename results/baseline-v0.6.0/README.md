# Tier-A codegen baseline (v0.6.0 / Phase 5 batch 0)

Per-distribution CSVs from `make bench-codegen-baseline` before any codegen Tier-B promotion.

Wave-1 candidates frozen at batch 0 (`poisson`, `gamma`, `beta`, `uniform`, `student-t`). Batch 1 extends the sweep to all parameterized codegen ids.

Compare during Phase 5:

```bash
make bench-codegen
python bench/compare_baseline.py --baseline results/baseline-v0.6.0 --geomean
```

Regenerate wave-1 baseline only (intentional re-baseline):

```bash
make bench-codegen-baseline
```

Full codegen sweep (batch 1):

```bash
make build
python bench/bench_codegen.py --out results/baseline-v0.6.0
```
