---
id: exponential-power
title: Exponential power distribution
aliases:
- Exponential power
type: continuous
support: real-line
category: continuous/whole-real-line
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Generalized%5Fnormal%5Fdistribution#Version%5F1
chart: assets/charts/exponential-power.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/real-line
---

> [!summary] In one sentence
> Exponential power distribution: A continuous random variable with support on the entire real line.

## When you meet it

- Specialized models where exponential power structure appears — see Wikipedia for domain examples.
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

![[exponential-power chart|400]](assets/charts/exponential-power.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Whole Real Line]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
