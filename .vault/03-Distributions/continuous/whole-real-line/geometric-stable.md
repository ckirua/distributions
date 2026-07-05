---
id: geometric-stable
title: Geometric stable distribution
aliases:
- Geometric stable
type: continuous
support: real-line
category: continuous/whole-real-line
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Geometric%5Fstable%5Fdistribution
chart: assets/charts/geometric-stable.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/real-line
---

> [!summary] In one sentence
> Geometric stable distribution: A continuous random variable with support on the entire real line.

## When you meet it

- Specialized models where geometric stable structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
P(X=k)=(1-p)^{k-1}p
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $4$ |
| Variance | $12$ |

## Shape

Default illustrative parameters are used in the plot below.

![[geometric-stable chart|400]](assets/charts/geometric-stable.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Whole Real Line]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
