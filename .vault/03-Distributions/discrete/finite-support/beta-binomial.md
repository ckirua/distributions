---
id: beta-binomial
title: Beta-binomial distribution
aliases:
- Beta-binomial
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: trials
  symbol: n
  role: Number of attempts
- name: Beta shape params
  symbol: \alpha,\beta
  role: Control over-dispersion of p
wikipedia: https://en.wikipedia.org/wiki/Beta-binomial%5Fdistribution
chart: assets/charts/beta-binomial.svg
code_status: planned
code_symbols:
  cpp: distributions::BetaBinomial
  python: cydist.beta_binomial
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Binomial-like counts when the success probability itself is random (Beta-distributed).

## When you meet it

- Over-dispersed success counts in heterogeneous populations.
- Hierarchical A/B tests with random conversion rates.
- Biological counts with extra variance beyond binomial.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $n$ | trials | $n\in\mathbb{N}$ | Number of attempts |
| $\alpha,\beta$ | Beta shape params | $>0$ | Control over-dispersion of p |

## Core functions

### PMF

$$
P(X=k)=\binom{n}{k}\frac{B(k+\alpha,n-k+\beta)}{B(\alpha,\beta)}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $n\alpha/(\alpha+\beta)$ |
| Variance | $see Wikipedia$ |
| Mode | $—$ |

## Shape

Default illustrative parameters are used in the plot below.

![[beta-binomial chart|400]](assets/charts/beta-binomial.svg)

## Relationships

- Related: [[Binomial]], [[Beta]], [[Negative binomial]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::BetaBinomial` (C++) and `cydist.beta_binomial` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
