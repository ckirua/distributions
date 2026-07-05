---
id: bernoulli
title: Bernoulli distribution
aliases:
- Bernoulli
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: success probability
  symbol: p
  role: Higher p shifts mass toward 1
wikipedia: https://en.wikipedia.org/wiki/Bernoulli%5Fdistribution
chart: assets/charts/bernoulli.svg
code_status: planned
code_symbols:
  cpp: distributions::Bernoulli
  python: cydist.bernoulli
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Models a single yes/no trial with fixed success probability.

## When you meet it

- A coin flip coded as 1 (heads) or 0 (tails).
- Whether a loan defaults in the next period.
- Indicator that a machine is up or down.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $p$ | success probability | $(0,1)$ | Higher p shifts mass toward 1 |

## Core functions

### PMF

$$
P(X=k)=p^k(1-p)^{1-k},\quad k\in\{0,1\}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $p$ |
| Variance | $p(1-p)$ |
| Mode | $1 if p>0.5 else 0$ |

## Shape

Default illustrative parameters are used in the plot below.

![[bernoulli chart|400]](assets/charts/bernoulli.svg)

## Relationships

- Related: [[Bernoulli]], [[Binomial]], [[Geometric]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Bernoulli` (C++) and `cydist.bernoulli` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
