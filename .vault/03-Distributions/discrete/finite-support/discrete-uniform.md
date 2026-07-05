---
id: discrete-uniform
title: Discrete uniform distribution
aliases:
- Discrete uniform
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: lower bound
  symbol: a
  role: Smallest possible value
- name: upper bound
  symbol: b
  role: Largest possible value
wikipedia: https://en.wikipedia.org/wiki/Discrete%5Funiform%5Fdistribution
chart: assets/charts/discrete-uniform.svg
code_status: planned
code_symbols:
  cpp: distributions::DiscreteUniform
  python: cydist.discrete_uniform
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Each of finitely many integers is equally likely.

## When you meet it

- Fair die roll.
- Random index into a fixed-size list.
- Uninformative choice among labeled buckets.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $a$ | lower bound | $a,b\in\mathbb{Z}$ | Smallest possible value |
| $b$ | upper bound | $a\le b$ | Largest possible value |

## Core functions

### PMF

$$
P(X=k)=\frac{1}{b-a+1},\ k=a,\ldots,b
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $(a+b)/2$ |
| Variance | $((b-a+1)^2-1)/12$ |
| Mode | $\text{non-unique}$ |

## Shape

Default illustrative parameters are used in the plot below.

![[discrete-uniform chart|400]](assets/charts/discrete-uniform.svg)

## Relationships

- Related: [[Categorical]], [[Binomial]], [[Uniform]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::DiscreteUniform` (C++) and `cydist.discrete_uniform` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
