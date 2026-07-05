# Agent batch checklist (visible GitHub progress)

After each heuristic implementation batch:

1. `make codegen && make build && make test && make test-sanity`
2. Update `IMPLEMENT_PROGRESS.md` (counts + batch table)
3. Commit on `v0.2.0`:
   ```bash
   git add -A
   git commit -m "Implement heuristic batch N: <category> (<k> samplers)."
   git push origin v0.2.0
   ```

Progress is visible on GitHub via commits to `v0.2.0` and CI runs on push.
