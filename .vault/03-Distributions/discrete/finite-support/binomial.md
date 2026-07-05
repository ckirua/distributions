---
id: binomial
title: Binomial distribution
aliases:
- Binomial
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: number of trials
  symbol: n
  role: Spreads support from 0 to n
- name: success probability
  symbol: p
  role: Skews the bell-shaped count distribution
wikipedia: https://en.wikipedia.org/wiki/Binomial%5Fdistribution
chart: assets/charts/binomial.svg
code_status: planned
code_symbols:
  cpp: distributions::Binomial
  python: cydist.binomial
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Counts successes in a fixed number of independent identical trials.

## When you meet it

- Number of filled orders out of 20 quotes.
- Defect count in a batch of inspected items.
- Click-through count from a fixed-size ad sample.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $n$ | number of trials | $n\in\mathbb{N}$ | Spreads support from 0 to n |
| $p$ | success probability | $(0,1)$ | Skews the bell-shaped count distribution |

## Core functions

### PMF

$$
P(X=k)=\binom{n}{k}p^k(1-p)^{n-k}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $np$ |
| Variance | $np(1-p)$ |
| Mode | $\lfloor (n+1)p\rfloor$ |

## Shape

Default illustrative parameters are used in the plot below.

![[binomial chart|400]](assets/charts/binomial.svg)

## Relationships

- Related: [[Bernoulli]], [[Poisson]], [[Normal]], [[Hypergeometric]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Binomial` (C++) and `cydist.binomial` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
