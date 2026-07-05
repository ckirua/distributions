---
id: gamma-gompertz
title: gamma/Gompertz distribution
aliases:
- gamma/Gompertz
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Gamma/Gompertz%5Fdistribution
chart: assets/charts/gamma-gompertz.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> gamma/Gompertz distribution: A continuous random variable with support on a half-line such as [0,∞).

## When you meet it

- Specialized models where gamma/gompertz structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)\propto x^{k-1}e^{-x/\theta}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $4$ |
| Variance | $8$ |

## Shape

Default illustrative parameters are used in the plot below.

![[gamma-gompertz chart|400]](assets/charts/gamma-gompertz.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
