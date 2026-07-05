#!/usr/bin/env python3
"""Generate Obsidian distribution and family notes from registry."""

from __future__ import annotations

from pathlib import Path

import yaml

from vault_common import VAULT, family_note_path, note_path
from vault_content import render_family, render_note


def write_bootstrap() -> None:
    paths = [
        VAULT / "00-Home",
        VAULT / "01-Concepts",
        VAULT / "02-Families",
        VAULT / "03-Distributions",
        VAULT / "04-Code",
        VAULT / "assets" / "charts",
        VAULT / "templates",
        VAULT / "_meta",
        VAULT / ".obsidian",
    ]
    for p in paths:
        p.mkdir(parents=True, exist_ok=True)

    (VAULT / ".obsidian" / "app.json").write_text(
        '{\n  "legacyEditor": false,\n  "showLineNumber": true,\n  "attachmentFolderPath": "assets"\n}\n',
        encoding="utf-8",
    )
    (VAULT / ".obsidian" / "core-plugins.json").write_text(
        '["file-explorer","global-search","graph","backlink","outgoing-link","tag-pane","page-preview","daily-notes","templates","note-composer","command-palette","editor-status","bookmarks","outline","word-count","file-recovery","sync","canvas"]\n',
        encoding="utf-8",
    )

    concept_pages = {
        "Probability distribution": """# Probability distribution

> [!summary]
> A **probability distribution** tells you which outcomes are possible and how likely each one is.

## Core idea

- Randomness becomes usable once you describe **support** (possible values) and **probabilities** (how mass is spread).
- Distributions are the vocabulary of statistics: same shape, different parameters → different contexts.

## Related

- [[Support and domain]]
- [[PMF, PDF, and CDF]]
- [[Parameters and moments]]
- [[Index]]
""",
        "Support and domain": """# Support and domain

> [!summary]
> The **support** is the set of values a random variable can take.

## Shapes of support

| Support type | Meaning | Examples |
|--------------|---------|----------|
| Finite discrete | Finitely many points | [[Binomial]], [[Categorical]] |
| Infinite discrete | Counts 0,1,2,… | [[Poisson]], [[Geometric]] |
| Bounded interval | Continuous on [a,b] | [[Beta]], [[Uniform]] |
| Semi-infinite | e.g. [0,∞) | [[Exponential]], [[Gamma]] |
| Whole real line | (−∞,∞) | [[Normal]], [[Cauchy]] |

Browse by support in the taxonomy MOCs linked from [[Index]].
""",
        "PMF, PDF, and CDF": """# PMF, PDF, and CDF

> [!summary]
> Three equivalent views of the same randomness: point masses, density, or accumulated probability.

## PMF (discrete)

For integer or categorical outcomes: $P(X=k)$ gives the probability of exactly $k$.

## PDF (continuous)

Density $f(x)$: probability of landing in a tiny interval is approximately $f(x)\\,dx$. Requires $\\int f(x)\\,dx = 1$.

## CDF

$F(x) = P(X \\le x)$ — always between 0 and 1, non-decreasing.

## Tip

If a formula looks unfamiliar, check the parameterization on the distribution's Wikipedia link — conventions differ by field.
""",
        "Parameters and moments": """# Parameters and moments

> [!summary]
> **Parameters** tune the shape; **moments** summarize location and spread.

## Common moments

| Moment | Symbol | Plain meaning |
|--------|--------|---------------|
| Mean | $\\mathbb{E}[X]$ | Long-run average |
| Variance | $\\mathrm{Var}(X)$ | Typical squared deviation from mean |
| Mode | — | Most likely value (if defined) |

## Parameters vs statistics

- **Parameter**: unknown constant in the model (e.g. true conversion rate).
- **Statistic**: computed from data (e.g. sample mean).

See [[Conjugate priors]] for Bayesian updating intuition.
""",
        "Conjugate priors": """# Conjugate priors

> [!summary]
> A **conjugate prior** is a prior distribution that, after seeing data, yields a posterior in the same family.

## Why it matters

You can update beliefs in closed form — useful for intuition even without computing posteriors yourself.

## Classic pairs

| Likelihood | Conjugate prior | Posterior |
|------------|-----------------|-----------|
| [[Binomial]] | [[Beta]] | Beta |
| [[Poisson]] | [[Gamma]] | Gamma |
| [[Normal]] (known variance) | [[Normal]] | Normal |

Individual distribution notes link to related families when relevant.
""",
        "Special cases and limits": """# Special cases and limits

> [!summary]
> Many distributions collapse into simpler ones when parameters take special values or when sample size grows.

## Examples

- [[Binomial]] with small $p$ and large $n$ → [[Poisson]] (law of rare events).
- [[Binomial]] with large $n$ → [[Normal]] (de Moivre–Laplace).
- [[Beta]] with $\\alpha=\\beta=1$ → [[Uniform]] on $(0,1)$.
- [[Geometric]] is a [[Negative binomial]] with $r=1$.

Use these links to navigate the catalog without memorizing every formula.
""",
        "Sampling vs density": """# Sampling vs density

> [!summary]
> A **density** (PMF/PDF) describes probabilities; **sampling** generates concrete random draws consistent with that density.

## Learning layer vs code layer

- This vault explains densities and intuition **without code**.
- The repository implementation (future `dist.hpp` / `cydist`) will provide fast **samplers** — functions whose names map back to these notes via [[Implementation index]].

## What to expect later

| Concept | Vault | Code (future) |
|---------|-------|---------------|
| What does it model? | Distribution note | — |
| PMF/PDF formula | Distribution note | — |
| Draw one value | — | `sample()` |
| Draw many values | — | `sample_batch()` |

When code ships, update `code_status` in frontmatter — indexes regenerate from `_meta/registry.yaml`.
""",
    }

    for title, body in concept_pages.items():
        (VAULT / "01-Concepts" / f"{title}.md").write_text(body, encoding="utf-8")

    (VAULT / "00-Home" / "Index.md").write_text(
        """# Probability Distributions Vault

Welcome. This vault mirrors the [Wikipedia probability-distributions template](https://en.wikipedia.org/wiki/Template:Probability_distributions).

## Start here

1. [[How to read this vault]]
2. [[Probability distribution]] — concepts without code
3. Taxonomy MOCs under `03-Distributions/` (regenerated from `_meta/registry.yaml`)

## Browse

### Concepts
- [[Support and domain]]
- [[PMF, PDF, and CDF]]
- [[Parameters and moments]]
- [[Conjugate priors]]
- [[Special cases and limits]]
- [[Sampling vs density]]

### Families
See `02-Families/` or the family list in each taxonomy MOC.

### Code bridge
- [[Implementation index]] — maps future C++/Python symbols to notes

## Regenerate indexes

From repo root (uses `.venv` build tools only — Obsidian itself needs no Python):

```bash
.venv/bin/python .vault/tools/build_vault.py
```
""",
        encoding="utf-8",
    )

    (VAULT / "00-Home" / "How to read this vault.md").write_text(
        """# How to read this vault

## Structure

| Folder | Purpose |
|--------|---------|
| `00-Home/` | Entry points |
| `01-Concepts/` | Statistical ideas explained once |
| `02-Families/` | Distribution families |
| `03-Distributions/` | One note per distribution, grouped like Wikipedia |
| `04-Code/` | Implementation cross-reference |
| `assets/charts/` | SVG plots |

## Each distribution note contains

1. One-sentence summary (callout)
2. Real-world scenarios
3. Parameter table
4. Core formulas (LaTeX)
5. Shape chart (when available)
6. Relationship wikilinks
7. Implementation footer (links to code when planned)

## Navigation tips

- Use **graph view** — clusters follow folder taxonomy.
- Follow **Related** links before diving into formulas.
- Check **Implementation index** when you wonder what a future function refers to.
""",
        encoding="utf-8",
    )

    (VAULT / "templates" / "Distribution.md").write_text(
        """---
id: 
title: 
aliases: []
type: discrete
support: finite
category: 
parameters: []
wikipedia: 
chart: 
code_status: none
code_symbols: {}
tags: []
---

> [!summary] In one sentence
> 

## When you meet it

- 

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|

## Core functions

## Shape

## Relationships

## Implementation

Track status on [[Implementation index]].
""",
        encoding="utf-8",
    )


def generate_notes(registry_path: Path) -> int:
    registry = yaml.safe_load(registry_path.read_text(encoding="utf-8"))
    count = 0
    for entry in registry["distributions"]:
        path = note_path(entry["category_path"], entry["id"])
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(render_note(entry), encoding="utf-8")
        count += 1
    for fam in registry["families"]:
        path = family_note_path(fam["id"])
        path.write_text(render_family(fam), encoding="utf-8")
        count += 1
    return count


def main() -> None:
    write_bootstrap()
    registry_path = VAULT / "_meta" / "registry.yaml"
    if not registry_path.exists():
        raise SystemExit("Run vault_parse.py first to create registry.yaml")
    n = generate_notes(registry_path)
    print(f"Wrote {n} notes")


if __name__ == "__main__":
    main()
