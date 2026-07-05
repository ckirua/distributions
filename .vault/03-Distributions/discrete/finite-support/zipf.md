---
id: zipf
title: Zipf distribution
aliases:
- Zipf
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: exponent
  symbol: s
  role: Larger s → steeper decay
- name: support size
  symbol: N
  role: Number of ranked categories
wikipedia: https://en.wikipedia.org/wiki/Zipf%27s%5Flaw
chart: assets/charts/zipf.svg
code_status: planned
code_symbols:
  cpp: distributions::Zipf
  python: cydist.zipf
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Power-law over ranks: common events dominate, rare events trail off slowly.

## When you meet it

- Word frequency in natural language.
- City population ranks.
- Web traffic by page popularity.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $s$ | exponent | $s>0$ | Larger s → steeper decay |
| $N$ | support size | $N\in\mathbb{N}$ | Number of ranked categories |

## Core functions

### PMF

$$
P(k)\propto k^{-s}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $depends on s,N$ |
| Variance | $depends on s,N$ |
| Mode | $1$ |

## Shape

Default illustrative parameters are used in the plot below.

![[zipf chart|400]](assets/charts/zipf.svg)

## Relationships

- Related: [[Zipf–Mandelbrot]], [[Yule–Simon]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Zipf` (C++) and `cydist.zipf` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
