# Tier-A codegen bench sweep (v0.7.0 batch 1)

Full parameterized codegen sweep @ 1k / 100k / 10M. Used to rank `tier_b_candidate` in registry.

```bash
make build
make bench-codegen-sweep          # full sweep (resumable)
make bench-codegen-sweep-resume   # continue after interrupt
python bench/rank_codegen_tiers.py
make codegen                      # sync registry fields
```

Regenerate ranking only:

```bash
python bench/rank_codegen_tiers.py --csv-dir results/codegen-tier-a
```
