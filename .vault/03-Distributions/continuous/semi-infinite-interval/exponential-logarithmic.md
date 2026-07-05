---
id: exponential-logarithmic
title: Exponential-logarithmic distribution
aliases:
- Exponential-logarithmic
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Exponential-logarithmic%5Fdistribution
chart: assets/charts/exponential-logarithmic.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> Exponential-logarithmic distribution: A continuous random variable with support on a half-line such as [0,∞).

## When you meet it

- Specialized models where exponential-logarithmic structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)=\lambda e^{-\lambda x},\ x\ge 0
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $1$ |
| Variance | $1$ |

## Shape

Default illustrative parameters are used in the plot below.

![[exponential-logarithmic chart|400]](assets/charts/exponential-logarithmic.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
