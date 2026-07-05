---
id: hypergeometric
title: Hypergeometric distribution
aliases:
- Hypergeometric
type: discrete
support: finite
category: discrete/finite-support
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Hypergeometric%5Fdistribution
chart: assets/charts/hypergeometric.svg
code_status: none
code_symbols: {}
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Hypergeometric distribution: A discrete random variable taking finitely many values.

## When you meet it

- Specialized models where hypergeometric structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PMF

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

![[hypergeometric chart|400]](assets/charts/hypergeometric.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
