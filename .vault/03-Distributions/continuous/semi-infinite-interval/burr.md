---
id: burr
title: Burr distribution
aliases:
- Burr
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Burr%5Fdistribution
chart: assets/charts/burr.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> Burr distribution: A continuous random variable with support on a half-line such as [0,∞).

## When you meet it

- Specialized models where burr structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### Density / mass function

> [!note] Formula
> See the [Wikipedia article](https://en.wikipedia.org/wiki/Burr%5Fdistribution) for the standard parameterization and density or mass function.

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $2.945$ |
| Variance | $see docs$ |

## Shape

Default illustrative parameters are used in the plot below.

![[burr chart|400]](assets/charts/burr.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
