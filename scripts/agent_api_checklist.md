# Agent API batch checklist (Phase 3)

After each API batch (see [`plan-api.md`](../plan-api.md)):

1. Read [`API_PROGRESS.md`](../API_PROGRESS.md) — implement **next batch only**.
2. Build & test (default build — must stay green):
   ```bash
   make codegen && make build && make test && make test-sanity
   ```
3. If batch touches float SIMD (batch 6+) or AVX-512 (batch 9):
   ```bash
   make build-simd && make test-simd
   make build-simd512 && make test-simd512   # batch 9 only
   ```
4. Update [`API_PROGRESS.md`](../API_PROGRESS.md):
   - status counts
   - hand-written tracker
   - completed batch section
5. Commit on `v0.5.0`:
   ```bash
   git add -A
   git commit -m "API batch N: <short summary>"
   git push origin v0.5.0
   ```

## Scope rules

- **Do** use `template<typename Sample>` + **`using Foo = Foo<default>`** aliases for backward compatibility.
- **Do** keep Tier A/B/C dispatch; Phase 3 extends dtypes, not replaces fast paths.
- **Do** use `compute_type_t<Sample>` (= `double`) for mixed-precision continuous math unless a batch says otherwise.
- **Do not** break cydist defaults (`float64` / `int32`) without an explicit batch.
- **Do not** add codegen template explosion in `recipes.py` before batch 7/8 without plan update.
- **Do not** remove existing struct names — alias to templated type instead.
- **Do** keep `make test-sanity` green.

## Loop prompt (Cursor)

```
Continue API batch from API_PROGRESS.md on branch v0.5.0.
Follow scripts/agent_api_checklist.md and plan-api.md.
```

Progress is visible on GitHub via commits to `v0.5.0` and CI runs on push.
