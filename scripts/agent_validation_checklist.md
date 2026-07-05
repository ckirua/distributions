# Agent validation batch checklist (Phase 4)

After each batch (see [`plan-validation.md`](../plan-validation.md)):

1. Read [`VALIDATION_PROGRESS.md`](../VALIDATION_PROGRESS.md) — implement **next batch only**.
2. Build & test:
   ```bash
   make codegen && make build && make test && make test-sanity
   ```
3. Update [`VALIDATION_PROGRESS.md`](../VALIDATION_PROGRESS.md).
4. Commit on `v0.6.0`:
   ```bash
   git add -A
   git commit -m "Validation batch N: <short summary>"
   git push origin v0.6.0
   ```

## Scope rules

- **Do** use `detail/validate.hpp` asserts in ctors (debug-only).
- **Do** add `validate_body` to codegen recipes where support is known.
- **Do not** add exceptions to Tier A/B/C hot paths unless already present.
- **Do not** implement two-pass batch or span refactors unless the batch says so.
- **Do** keep `make test-sanity` green.

## Loop prompt (Cursor)

```
Continue validation batch from VALIDATION_PROGRESS.md on branch v0.6.0.
Follow scripts/agent_validation_checklist.md and plan-validation.md.
```
