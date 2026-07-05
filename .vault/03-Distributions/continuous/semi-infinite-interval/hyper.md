---
id: hyper
title: Hyper distribution
aliases:
- Hyper
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Hyper-Erlang%5Fdistribution
chart: assets/charts/hyper.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> Hyper distribution: A continuous random variable with support on a half-line such as [0,∞).

## When you meet it

- Specialized models where hyper structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
P(X=k)=\frac{\binom{K}{k}\binom{N-K}{n-k}}{\binom{N}{n}}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $1$ |
| Variance | $0.7347$ |

## Shape

Default illustrative parameters are used in the plot below.

![[hyper chart|400]](assets/charts/hyper.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
