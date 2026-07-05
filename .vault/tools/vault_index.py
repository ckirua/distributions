#!/usr/bin/env python3
"""Regenerate taxonomy MOC pages and Implementation index from registry."""

from __future__ import annotations

from collections import defaultdict
from pathlib import Path

import yaml

from vault_common import VAULT

MOC_TITLES = {
    "discrete/finite-support": "Discrete — finite support",
    "discrete/infinite-support": "Discrete — infinite support",
    "continuous/bounded-interval": "Continuous — bounded interval",
    "continuous/semi-infinite-interval": "Continuous — semi-infinite interval",
    "continuous/whole-real-line": "Continuous — whole real line",
    "continuous/variable-support": "Continuous — variable support",
    "mixed/continuous-discrete": "Mixed — continuous-discrete",
    "multivariate/discrete": "Multivariate — discrete",
    "multivariate/continuous": "Multivariate — continuous",
    "multivariate/matrix-valued": "Multivariate — matrix-valued",
    "directional/univariate-circular": "Directional — univariate circular",
    "directional/bivariate-spherical": "Directional — bivariate spherical",
    "directional/bivariate-toroidal": "Directional — bivariate toroidal",
    "directional/multivariate": "Directional — multivariate",
    "degenerate-and-singular/degenerate": "Degenerate",
    "degenerate-and-singular/singular": "Singular",
}


def _param_summary(entry: dict) -> str:
    params = entry.get("parameters_meta") or entry.get("parameters") or []
    if not params:
        return "—"
    if isinstance(params[0], dict):
        return ", ".join(p.get("symbol", "?") for p in params[:3])
    return ", ".join(p[0] for p in params[:3])


def render_moc(category_path: str, entries: list[dict]) -> str:
    title = MOC_TITLES.get(category_path, category_path)
    rows = []
    for e in sorted(entries, key=lambda x: x["title"].lower()):
        params = _param_summary(e)
        chart = "yes" if e.get("plottable") else "—"
        rows.append(
            f"| [[{e['id']}]] | {e['title']} | {e['support']} | {params} | {e.get('code_status', 'none')} | {chart} |"
        )
    body = "\n".join(rows)
    return f"""# {title}

> [!note] Taxonomy MOC
> Auto-generated from `_meta/registry.yaml`. Regenerate with `.vault/tools/build_vault.py`.

| Note | Name | Support | Key params | Code | Chart |
|------|------|---------|------------|------|-------|
{body}

Back to [[Index]].
"""


def render_implementation_index(entries: list[dict]) -> str:
    planned = [e for e in entries if e.get("code_status") == "planned"]
    planned.sort(key=lambda x: x["title"].lower())
    rows = []
    for e in planned:
        cpp = e.get("code_symbols", {}).get("cpp", "—")
        py = e.get("code_symbols", {}).get("python", "—")
        rows.append(f"| [[{e['id']}]] | `{cpp}` | `{py}` | {e.get('code_status', 'none')} |")
    other = len(entries) - len(planned)
    body = "\n".join(rows) if rows else "| — | — | — | — |"
    return f"""# Implementation index

> [!summary]
> Maps future repository symbols to vault learning notes. Status values: `none` → `planned` → `benchmarked` → `implemented`.

## Sync convention

1. Source of truth for vault metadata: [`_meta/registry.yaml`](../_meta/registry.yaml)
2. When `dist.hpp` / `cydist` adds a sampler, set `code_status: implemented` on that distribution's frontmatter (or update registry and rebuild).
3. Re-run `.venv/bin/python .vault/tools/build_vault.py` to refresh this table and MOCs.
4. Benchmark results stay in repo `results/` — not duplicated here.

## Phase 1 planned samplers

| Distribution | C++ symbol | Python symbol | Status |
|--------------|------------|---------------|--------|
{body}

## Catalog coverage

- **{len(entries)}** distribution notes in vault
- **{len(planned)}** with planned code symbols
- **{other}** learning-only (no current implementation plan)

See [[Sampling vs density]] for how notes relate to future `sample()` / `sample_batch()` functions.
"""


def render_code_readme() -> str:
    return """# Code bridge

- [[Implementation index]] — symbol lookup
- [[Sampling vs density]] — conceptual map to samplers

Update workflow documented on [[Implementation index]].
"""


def build_indexes(registry_path: Path) -> None:
    registry = yaml.safe_load(registry_path.read_text(encoding="utf-8"))
    entries = registry["distributions"]
    by_cat: dict[str, list[dict]] = defaultdict(list)
    for e in entries:
        by_cat[e["category_path"]].append(e)

    for category_path, group in by_cat.items():
        folder = VAULT / "03-Distributions" / category_path
        folder.mkdir(parents=True, exist_ok=True)
        moc = folder / "_MOC.md"
        moc.write_text(render_moc(category_path, group), encoding="utf-8")

    code_dir = VAULT / "04-Code"
    code_dir.mkdir(parents=True, exist_ok=True)
    (code_dir / "Implementation index.md").write_text(
        render_implementation_index(entries), encoding="utf-8"
    )
    (code_dir / "Code bridge.md").write_text(render_code_readme(), encoding="utf-8")
    print(f"Indexes: {len(by_cat)} MOCs + implementation index")


def main() -> None:
    build_indexes(VAULT / "_meta" / "registry.yaml")


if __name__ == "__main__":
    main()
