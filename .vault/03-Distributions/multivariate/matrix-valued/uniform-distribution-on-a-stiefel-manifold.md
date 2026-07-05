---
id: uniform-distribution-on-a-stiefel-manifold
title: Uniform distribution on a Stiefel manifold distribution
aliases:
- Uniform distribution on a Stiefel manifold
- Uniform on a Stiefel manifold
type: multivariate
support: matrix
category: multivariate/matrix-valued
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Uniform%5Fdistribution%5Fon%5Fa%5FStiefel%5Fmanifold
chart: assets/charts/uniform-distribution-on-a-stiefel-manifold.svg
code_status: none
code_symbols: {}
tags:
- distribution/multivariate
- support/matrix
---

> [!summary] In one sentence
> Uniform distribution on a Stiefel manifold distribution: A distribution over random matrices (covariance-like objects).

## When you meet it

- Specialized models where uniform distribution on a stiefel manifold structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)=\frac{1}{b-a},\ x\in[a,b]
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $0.5$ |
| Variance | $0.08333$ |

## Shape

Default illustrative parameters are used in the plot below.

![[uniform-distribution-on-a-stiefel-manifold chart|400]](assets/charts/uniform-distribution-on-a-stiefel-manifold.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Matrix Valued]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
