# Parameter validation plan (Phase 4)

**Prerequisite:** Phase 3 typed API complete on `v0.5.0`.

**Branch:** `v0.6.0` from `v0.5.0`.

**Goals:** Catch invalid support/parameters before silent NaNs in MC paths. Zero overhead in release (`assert` / `NDEBUG`). Optional Python `ValueError` at cydist boundary.

**Non-goals (this phase):**

- Two-pass batch for heavy codegen samplers (profile first; separate track)
- `std::span` churn across all 189 headers (shim-only when touched)
- Exceptions in C++ hot ctors (except existing categorical empty-vector throw)

---

## Batches

| Batch | Scope |
|------:|-------|
| 0 | `detail/validate.hpp`, codegen `validate_body`, tracker + checklist |
| 1 | Hand-written 13 — debug asserts in ctors |
| 2 | Codegen family — common patterns (`p∈[0,1]`, `α,β>0`, `a>|b|`, …) |
| 3 | cydist Python `ValueError` for hand-written + high-traffic ids |
| 4 | `std::span` at cydist C shim boundary (optional) |

**Commit pattern:** `Validation batch N: <summary>`

---

## Agent loop

See [`scripts/agent_validation_checklist.md`](scripts/agent_validation_checklist.md).

```
Continue validation batch from VALIDATION_PROGRESS.md on branch v0.6.0.
Follow scripts/agent_validation_checklist.md and plan-validation.md.
```

Prior phases: [`OPTIMIZE_PROGRESS.md`](OPTIMIZE_PROGRESS.md), [`SIMD_PROGRESS.md`](SIMD_PROGRESS.md), [`API_PROGRESS.md`](API_PROGRESS.md).
