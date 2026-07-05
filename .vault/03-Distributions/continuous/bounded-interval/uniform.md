---
id: uniform
title: Uniform distribution
aliases:
- Uniform
type: continuous
support: bounded
category: continuous/bounded-interval
parameters:
- name: lower bound
  symbol: a
  role: Left edge of support
- name: upper bound
  symbol: b
  role: Right edge of support
wikipedia: https://en.wikipedia.org/wiki/Continuous%5Funiform%5Fdistribution
chart: assets/charts/uniform.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/bounded
---

> [!summary] In one sentence
> Every value in an interval is equally likely — maximum ignorance on a bounded range.

## When you meet it

- Random offset in a known window.
- Bid shading within a tick band.
- Placeholder prior when only bounds are known.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $a$ | lower bound | $a<b$ | Left edge of support |
| $b$ | upper bound | $a<b$ | Right edge of support |

## Core functions

### PDF

$$
f(x)=\frac{1}{b-a},\quad x\in[a,b]
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $(a+b)/2$ |
| Variance | $(b-a)^2/12$ |
| Mode | $any point in [a,b]$ |

## Shape

Default illustrative parameters are used in the plot below.

![[uniform chart|400]](assets/charts/uniform.svg)

## Relationships

- Related: [[Beta]], [[Triangular]]
- Taxonomy: [[Bounded Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
