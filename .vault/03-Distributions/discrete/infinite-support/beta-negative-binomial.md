---
id: beta-negative-binomial
title: Beta negative binomial distribution
aliases:
- Beta negative binomial
type: discrete
support: infinite
category: discrete/infinite-support
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Beta%5Fnegative%5Fbinomial%5Fdistribution
chart: assets/charts/beta-negative-binomial.svg
code_status: none
code_symbols: {}
tags:
- distribution/discrete
- support/infinite
---

> [!summary] In one sentence
> Beta negative binomial distribution: A discrete random variable on the non-negative integers or similar count set.

## When you meet it

- Specialized models where beta negative binomial structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PMF

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

![[beta-negative-binomial chart|400]](assets/charts/beta-negative-binomial.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Infinite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
