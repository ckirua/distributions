# Vault build tools

Python scripts that **generate** markdown notes, charts, and indexes. Obsidian only reads the output — it never runs these.

## Setup

From repo root (shared `.venv` with the library):

```bash
.venv/bin/pip install -r .vault/requirements.txt
```

Or a vault-only venv:

```bash
cd .vault
python -m venv .venv
.venv/bin/pip install -r requirements.txt
```

## Rebuild

From repo root:

```bash
.venv/bin/python .vault/tools/build_vault.py
```

From inside `.vault/`:

```bash
.venv/bin/python tools/build_vault.py
```

## Files

| Script | Purpose |
|--------|---------|
| `build_vault.py` | Full pipeline (parse → notes → charts → indexes) |
| `vault_parse.py` | Wikipedia template → `_meta/registry.yaml` |
| `vault_generate.py` | Distribution + family markdown notes |
| `vault_charts.py` | SVG plots in `assets/charts/` |
| `vault_index.py` | Taxonomy `_MOC.md` + Implementation index |
| `vault_common.py` | Paths, slugs, code symbol map |
| `vault_content.py` | Note prose and formulas |
| `vault_scipy.py` | Scipy mapping for charts and auto-enrichment |

Sampler `code_status` and symbols live in `_meta/registry.yaml`. C++ codegen: [`tools/README.md`](../../tools/README.md) at repo root (`make codegen`).
