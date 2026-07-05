---
id: wishart
title: Wishart distribution
aliases:
- Wishart
type: multivariate
support: matrix
category: multivariate/matrix-valued
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Wishart%5Fdistribution
chart: assets/charts/wishart.svg
code_status: none
code_symbols: {}
tags:
- distribution/multivariate
- support/matrix
---

> [!summary] In one sentence
> Wishart distribution: A distribution over random matrices (covariance-like objects).

## When you meet it

- Specialized models where wishart structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### Density / mass function

> [!note] Formula
> See the [Wikipedia article](https://en.wikipedia.org/wiki/Wishart%5Fdistribution) for the standard parameterization and density or mass function.

### Moments

| Quantity | Value |
|----------|-------|
| Mean | See [[Parameters and moments]] |

## Shape

Default illustrative parameters are used in the plot below.

![[wishart chart|400]](assets/charts/wishart.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Matrix Valued]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
