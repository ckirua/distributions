---
id: exponential
title: Exponential distribution
aliases:
- Exponential
type: continuous
support: semi-infinite
category: continuous/semi-infinite-interval
parameters:
- name: rate
  symbol: λ
  role: Larger λ → shorter typical waits
wikipedia: https://en.wikipedia.org/wiki/Exponential%5Fdistribution
chart: assets/charts/exponential.svg
code_status: planned
code_symbols:
  cpp: distributions::Exponential
  python: cydist.exponential
tags:
- distribution/continuous
- support/semi-infinite
---

> [!summary] In one sentence
> Waiting time until the next event when events occur at a constant rate.

## When you meet it

- Time until the next trade tick.
- Lifetime of a component with constant failure rate.
- Gap between customer arrivals.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $λ$ | rate | $\lambda>0$ | Larger λ → shorter typical waits |

## Core functions

### PDF

$$
f(x)=\lambda e^{-\lambda x},\quad x\ge 0
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $1/\lambda$ |
| Variance | $1/\lambda^2$ |
| Mode | $0$ |

## Shape

Default illustrative parameters are used in the plot below.

![[exponential chart|400]](assets/charts/exponential.svg)

## Relationships

- Related: [[Poisson]], [[Gamma]], [[Weibull]]
- Taxonomy: [[Semi Infinite Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Exponential` (C++) and `cydist.exponential` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
