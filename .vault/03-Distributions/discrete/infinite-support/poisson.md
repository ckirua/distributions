---
id: poisson
title: Poisson distribution
aliases:
- Poisson
type: discrete
support: infinite
category: discrete/infinite-support
parameters:
- name: rate (expected count)
  symbol: λ
  role: Mean and variance both equal λ
wikipedia: https://en.wikipedia.org/wiki/Poisson%5Fdistribution
chart: assets/charts/poisson.svg
code_status: none
code_symbols: {}
tags:
- distribution/discrete
- support/infinite
---

> [!summary] In one sentence
> Models event counts in a fixed interval when events arrive independently at a steady average rate.

## When you meet it

- Number of trades arriving in one second.
- Website visits per minute.
- Rare defect arrivals on a production line.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $λ$ | rate (expected count) | $\lambda>0$ | Mean and variance both equal λ |

## Core functions

### PMF

$$
P(X=k)=\frac{e^{-\lambda}\lambda^k}{k!},\quad k=0,1,2,\ldots
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $\lambda$ |
| Variance | $\lambda$ |
| Mode | $\lfloor\lambda\rfloor$ |

## Shape

Default illustrative parameters are used in the plot below.

![[poisson chart|400]](assets/charts/poisson.svg)

## Relationships

- Related: [[Exponential]], [[Negative binomial]], [[Binomial]]
- Taxonomy: [[Infinite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
