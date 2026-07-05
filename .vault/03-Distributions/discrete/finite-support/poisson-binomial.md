---
id: poisson-binomial
title: Poisson binomial distribution
aliases:
- Poisson binomial
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: trial probabilities
  symbol: p_i
  role: Each trial may differ
wikipedia: https://en.wikipedia.org/wiki/Poisson%5Fbinomial%5Fdistribution
chart: assets/charts/poisson-binomial.svg
code_status: planned
code_symbols:
  cpp: distributions::PoissonBinomial
  python: cydist.poisson_binomial
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Sum of independent Bernoulli trials with non-identical success probabilities.

## When you meet it

- Filled orders when each quote has its own fill probability.
- Portfolio defaults with name-specific probabilities.
- Network packet success with varying link quality.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $p_i$ | trial probabilities | $p_i\in[0,1]$ | Each trial may differ |

## Core functions

### PMF

$$
P(X=k)=\sum_{A:\ |A|=k}\prod_{i\in A}p_i\prod_{j\notin A}(1-p_j)
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $\sum p_i$ |
| Variance | $\sum p_i(1-p_i)$ |
| Mode | $—$ |

## Shape

Default illustrative parameters are used in the plot below.

![[poisson-binomial chart|400]](assets/charts/poisson-binomial.svg)

## Relationships

- Related: [[Binomial]], [[Poisson]], [[Bernoulli]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::PoissonBinomial` (C++) and `cydist.poisson_binomial` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
