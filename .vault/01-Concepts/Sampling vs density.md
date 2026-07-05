# Sampling vs density

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
