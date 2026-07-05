---
id: pareto
title: Pareto distribution
aliases:
- Pareto
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Pareto%5Fdistribution
chart: assets/charts/pareto.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> Pareto distribution: A continuous random variable with support on a half-line such as [0,∞).

## When you meet it

- Specialized models where pareto structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)=\frac{\alpha x_m^\alpha}{x^{\alpha+1}},\ x\ge x_m
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $2$ |
| Variance | $see docs$ |

## Shape

Default illustrative parameters are used in the plot below.

![[pareto chart|400]](assets/charts/pareto.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
