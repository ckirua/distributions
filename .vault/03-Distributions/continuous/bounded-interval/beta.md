---
id: beta
title: Beta distribution
aliases:
- Beta
type: continuous
support: bounded
category: continuous/bounded-interval
parameters:
- name: shape 1
  symbol: α
  role: With β controls skew on (0,1)
- name: shape 2
  symbol: β
  role: Equal α=β gives a symmetric hump
wikipedia: https://en.wikipedia.org/wiki/Beta%5Fdistribution
chart: assets/charts/beta.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/bounded
---

> [!summary] In one sentence
> Flexible distribution on the unit interval for proportions, probabilities, and rates bounded between 0 and 1.

## When you meet it

- Prior belief about a conversion rate.
- Random completion percentage.
- Probability of default bounded in (0,1).

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $α$ | shape 1 | $\alpha>0$ | With β controls skew on (0,1) |
| $β$ | shape 2 | $\beta>0$ | Equal α=β gives a symmetric hump |

## Core functions

### PDF

$$
f(x)=\frac{x^{\alpha-1}(1-x)^{\beta-1}}{B(\alpha,\beta)},\quad x\in(0,1)
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $\alpha/(\alpha+\beta)$ |
| Variance | $\alpha\beta/[(\alpha+\beta)^2(\alpha+\beta+1)]$ |
| Mode | $(\alpha-1)/(\alpha+\beta-2) if \alpha,\beta>1$ |

## Shape

Default illustrative parameters are used in the plot below.

![[beta chart|400]](assets/charts/beta.svg)

## Relationships

- Related: [[Uniform]], [[Binomial]], [[Beta-binomial]]
- Taxonomy: [[Bounded Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
