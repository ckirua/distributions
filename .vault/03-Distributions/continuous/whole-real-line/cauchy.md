---
id: cauchy
title: Cauchy distribution
aliases:
- Cauchy
type: continuous
support: real-line
category: continuous/whole-real-line
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Cauchy%5Fdistribution
chart: assets/charts/cauchy.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/real-line
---

> [!summary] In one sentence
> Cauchy distribution: A continuous random variable with support on the entire real line.

## When you meet it

- Specialized models where cauchy structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)=\frac{1}{\pi\gamma[1+((x-x_0)/\gamma)^2]}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $see docs$ |
| Variance | $see docs$ |

## Shape

Default illustrative parameters are used in the plot below.

![[cauchy chart|400]](assets/charts/cauchy.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Whole Real Line]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
