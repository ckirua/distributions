---
id: logistic
title: Logistic distribution
aliases:
- Logistic
type: continuous
support: real-line
category: continuous/whole-real-line
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Logistic%5Fdistribution
chart: assets/charts/logistic.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/real-line
---

> [!summary] In one sentence
> Logistic distribution: A continuous random variable with support on the entire real line.

## When you meet it

- Specialized models where logistic structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)=\frac{e^{-(x-\mu)/s}}{s(1+e^{-(x-\mu)/s})^2}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $0$ |
| Variance | $3.29$ |

## Shape

Default illustrative parameters are used in the plot below.

![[logistic chart|400]](assets/charts/logistic.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Whole Real Line]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
