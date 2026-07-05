---
id: chi-squared
title: Chi-squared distribution
aliases:
- Chi-squared
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Chi-squared%5Fdistribution
chart: assets/charts/chi-squared.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> Chi-squared distribution: A continuous random variable with support on a half-line such as [0,∞).

## When you meet it

- Specialized models where chi-squared structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)\ \text{with}\ k\ \text{degrees of freedom}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $3$ |
| Variance | $6$ |

## Shape

Default illustrative parameters are used in the plot below.

![[chi-squared chart|400]](assets/charts/chi-squared.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
