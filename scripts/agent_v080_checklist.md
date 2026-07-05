# Agent v0.8.0 batch checklist (Phase 6 → v1.0.0)

**Prerequisite:** [`v0.7.0`](https://github.com/ckirua/distributions/tree/v0.7.0) merged to `main`.

After each batch (see [`plan-v0.8.0.md`](../plan-v0.8.0.md)):

1. Read [`V080_PROGRESS.md`](../V080_PROGRESS.md) — implement **next batch only**.
2. Build & test:
   ```bash
   make codegen && make build && make test && make test-sanity
   ```
3. If batch touches Tier B, table fast paths, or registry:
   ```bash
   .venv/bin/python bench/bench_codegen.py --wave3 --out results/current
   # compare vs codegen-tier-a for wave 3 ids
   # batch 1: re-bench continuous-bernoulli + discrete-weibull vs prior Tier B
   ```
4. Update [`V080_PROGRESS.md`](../V080_PROGRESS.md).
5. Commit on `v0.8.0`:
   ```bash
   git add -A
   git commit -m "v0.8.0 batch N: <short summary>"
   git push origin v0.8.0
   ```

## Batch 4 (release) only

```bash
gh pr create --base main --head v0.8.0 --title "v1.0.0: Tier B wave 3 + table sampler polish"
# after merge on main:
git tag v1.0.0
git push origin v1.0.0
```

## Scope rules

- **Do** promote Tier B via composition hooks reusing `detail::fast/` primitives.
- **Do** require ≥ **1.5×** @10M vs Tier-A before shipping new fast paths.
- **Do** require ≥ **1.2×** vs current Tier B before shipping alias/binary-search changes.
- **Do not** add parallel SIMD or profile-driven queue bulk promotion (post-v1).
- **Do not** break sanity tests or cydist ABI.
- **Do** keep `make test-all` green before batch 3 sign-off.

## Loop prompt (Cursor)

```
Continue v0.8.0 batch from V080_PROGRESS.md on branch v0.8.0.
Follow scripts/agent_v080_checklist.md and plan-v0.8.0.md.
```

Progress: commits to `v0.8.0` + CI on push → tag **v1.0.0** at batch 4.
