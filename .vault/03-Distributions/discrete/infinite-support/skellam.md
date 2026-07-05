---
id: skellam
title: Skellam distribution
aliases:
- Skellam
type: discrete
support: infinite
category: discrete/infinite-support
parameters:
- name: Poisson rate 1
  symbol: \mu_1
  role: Upward pressure on difference
- name: Poisson rate 2
  symbol: \mu_2
  role: Downward pressure on difference
wikipedia: https://en.wikipedia.org/wiki/Skellam%5Fdistribution
chart: assets/charts/skellam.svg
code_status: planned
code_symbols:
  cpp: distributions::Skellam
  python: cydist.skellam
tags:
- distribution/discrete
- support/infinite
---

> [!summary] In one sentence
> Difference of two independent Poisson counts.

## When you meet it

- Net order flow (buys minus sells) in a window.
- Goal difference when two Poisson processes compete.
- Inventory change from arrivals minus departures.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $\mu_1$ | Poisson rate 1 | $>0$ | Upward pressure on difference |
| $\mu_2$ | Poisson rate 2 | $>0$ | Downward pressure on difference |

## Core functions

### PMF

$$
P(X=k)=e^{-(\mu_1+\mu_2)}(\mu_1/\mu_2)^{k/2}I_{|k|}(2\sqrt{\mu_1\mu_2})
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $\mu_1-\mu_2$ |
| Variance | $\mu_1+\mu_2$ |
| Mode | $\lfloor\mu_1-\mu_2\rfloor$ |

## Shape

Default illustrative parameters are used in the plot below.

![[skellam chart|400]](assets/charts/skellam.svg)

## Relationships

- Related: [[Poisson]], [[Normal (Gaussian)]]
- Taxonomy: [[Infinite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Skellam` (C++) and `cydist.skellam` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
