---
id: beta-prime
title: Beta prime distribution
aliases:
- Beta prime
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Beta%5Fprime%5Fdistribution
chart: assets/charts/beta-prime.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> Beta prime distribution: A continuous random variable with support on a half-line such as [0,∞).

## When you meet it

- Specialized models where beta prime structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)\propto x^{\alpha-1}(1-x)^{\beta-1}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $0.2857$ |
| Variance | $0.02551$ |

## Shape

Default illustrative parameters are used in the plot below.

![[beta-prime chart|400]](assets/charts/beta-prime.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
