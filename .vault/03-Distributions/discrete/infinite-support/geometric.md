---
id: geometric
title: Geometric distribution
aliases:
- Geometric
type: discrete
support: infinite
category: discrete/infinite-support
parameters:
- name: success probability
  symbol: p
  role: Smaller p → longer tail of large counts
wikipedia: https://en.wikipedia.org/wiki/Geometric%5Fdistribution
chart: assets/charts/geometric.svg
code_status: planned
code_symbols:
  cpp: distributions::Geometric
  python: cydist.geometric
tags:
- distribution/discrete
- support/infinite
---

> [!summary] In one sentence
> Number of trials until the first success in repeated independent Bernoulli trials.

## When you meet it

- Trials until first conversion in a funnel.
- Packets sent until first acknowledgement.
- Attempts until first successful login.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $p$ | success probability | $(0,1]$ | Smaller p → longer tail of large counts |

## Core functions

### PMF

$$
P(X=k)=(1-p)^{k-1}p,\quad k=1,2,\ldots
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $1/p$ |
| Variance | $(1-p)/p^2$ |
| Mode | $1$ |

## Shape

Default illustrative parameters are used in the plot below.

![[geometric chart|400]](assets/charts/geometric.svg)

## Relationships

- Related: [[Bernoulli]], [[Negative binomial]], [[Exponential]]
- Taxonomy: [[Infinite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Geometric` (C++) and `cydist.geometric` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
