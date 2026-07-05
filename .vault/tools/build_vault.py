#!/usr/bin/env python3
"""Build the full Obsidian vault (parse → notes → charts → indexes)."""

from __future__ import annotations

import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

from vault_charts import main as charts_main
from vault_generate import generate_notes, write_bootstrap
from vault_index import build_indexes
from vault_parse import parse_template, write_registry, write_taxonomy
from vault_common import TEMPLATE_MD, VAULT


def main() -> None:
    write_bootstrap()
    text = TEMPLATE_MD.read_text(encoding="utf-8")
    distributions, families = parse_template(text)
    write_taxonomy(distributions, families)
    write_registry(distributions, families)
    print(f"Parsed {len(distributions)} distributions, {len(families)} families")

    n = generate_notes(VAULT / "_meta" / "registry.yaml")
    print(f"Generated {n} markdown notes")

    charts_main()
    build_indexes(VAULT / "_meta" / "registry.yaml")
    print("Vault build complete:", VAULT)


if __name__ == "__main__":
    main()
