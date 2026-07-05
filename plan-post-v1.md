# Post-v1.0.0 roadmap — profile-driven optimization loop

**Prerequisite:** [`v1.0.0`](plan-v0.8.0.md) tagged on `main` (Phase 6 complete).

Phases 1–6 delivered: hand-written Tier B/C (13 core), codegen validation, selective codegen Tier B (target ≥9), typed `Sample` templates, cydist completeness. **Everything below is optional and profile-driven** — no fixed batch count until a phase is chartered.

---

## Decision gates (same as Phases 1–2)

| Gate | Ship | Skip |
|------|------|------|
| Tier B vs Tier-A @10M | ≥ **1.5×** | < 1.5× or regression |
| Tier C vs Tier B @10M | ≥ **1.2×** | regression or no AVX |
| Parallel vs single-thread @10M | ≥ **1.3×** on ≥8 cores | Amdahl / repro risk |
| Sanity (scipy) | pass | any fail |

Baselines: `results/codegen-tier-a/` (Tier-A), `results/baseline-v1.0.0/` (Tier-B release set), `results/baseline-v0.2.0/` (hand-written cumulative).

---

## Track A — Codegen Tier B (profile-driven)

**Source queue:** remaining **`tier_b_candidate`** ids in `.vault/_meta/registry.yaml` (~14 after v0.8.0 wave 3).

**Workflow (repeat per agent session):**

1. `make bench-codegen-sweep-resume` or profile hot ids from user workload.
2. Pick **1–3** ids; prototype hook in `detail::fast/` or two-pass batch.
3. Bench @10M vs `codegen-tier-a`; ship only if ≥ 1.5×.
4. Update registry `batch_fast`; `make codegen`; `make test-all`.
5. Commit: `post-v1 Tier B: <id>` (no version branch required — trunk or short `opt/<id>` branches).

**Hard skips (until redesigned):**

- Multivariate / matrix batch (`wishart`, `dirichlet`, …) — needs batched linear algebra, not scalar `out[n]`.
- Heavy rejection without profile win (`folded-normal`, `truncated-normal`, …).
- Near-miss retries only after algorithm change (not parameter tweak).

---

## Track B — float / wider Sample on codegen

**Goal:** `GammaDistribution<float>` Tier B where ROI matches hand-written normal/exponential.

1. Extend `if constexpr` dispatch: `Sample == float` → float fast path or compute-double-store-float.
2. cydist: optional `float32` out for high-traffic codegen ids (mirror Phase 3 FusedType pattern).
3. Bench + `test_reproducibility` statistical checks (not byte repro).

---

## Track C — Tier C SIMD (codegen subset)

**Goal:** AVX2/libmvec on **2–3** hot continuous codegen ids only (not 171).

1. `DISTRIBUTIONS_ENABLE_SIMD` dispatch in generated `sample_batch` (mirror hand-written).
2. Ship only if Tier C beats Tier B ≥ 1.2× @10M and repro tests pass.

---

## Track D — Parallel batch wrapper

**Goal:** [`batch_parallel.hpp`](plan.md) — chunk `sample_batch` with derived sub-seeds.

**Gate (from Phase 1 batch 6):** only if single-thread geomean insufficient **and** workloads use **n ≥ 10M** on **≥8 cores**.

1. OpenMP or `std::thread` chunk loop; reproducible sub-seed policy documented.
2. Bench strong scaling; ship if ≥ 1.3× @10M on target hardware.
3. No change to Tier A/B single-thread paths.

---

## Track E — Infrastructure / ergonomics

| Item | Trigger |
|------|---------|
| Alias method for zeta / zipf-like discrete | profile shows table scan bound |
| `Sampler` concept tests for all Tier B ids | when Tier B count > 15 |
| CI bench regression job | optional; compare PR vs `baseline-v1.0.0` |
| `long double` / `std::floating_point` | explicit user request + per-type fast paths |

---

## Agent loop (post-v1, no fixed phase number)

```
Read plan-post-v1.md. Pick one track (A–E) from user or profile data.
One id or one primitive per session. Bench-gated ship. make test-all before push.
```

Trackers: append to [`V080_PROGRESS.md`](V080_PROGRESS.md) or create `OPTIMIZE_V2_PROGRESS.md` when Track A ships ≥3 ids post-v1.

---

## Version branch policy after v1.0.0

| Work type | Branch |
|-----------|--------|
| Bench-gated Tier B (1–3 ids) | trunk or `opt/<id>` |
| Large track (parallel, SIMD codegen sweep) | `v0.x.0` charter + plan file |
| Release | tag `v1.x.0` when chartered phase completes |

No standing version branches unless a new chartered phase starts (same pattern as v0.2.0–v0.8.0).
