---
id: categorical
title: Categorical distribution
aliases:
- Categorical
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: category probabilities
  symbol: p_i
  role: Higher p_i raises category i mass
wikipedia: https://en.wikipedia.org/wiki/Categorical%5Fdistribution
chart: assets/charts/categorical.svg
code_status: planned
code_symbols:
  cpp: distributions::Categorical
  python: cydist.categorical
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> One draw from finitely many labeled outcomes, generalizing Bernoulli to K categories.

## When you meet it

- Which product category a customer buys.
- Predicted class label from a classifier.
- Routing an order to one of several venues.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $p_i$ | category probabilities | $\sum p_i=1$ | Higher p_i raises category i mass |

## Core functions

### PMF

$$
P(X=i)=p_i
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $— (categorical)$ |
| Variance | $—$ |
| Mode | $\arg\max p_i$ |

## Shape

Default illustrative parameters are used in the plot below.

![[categorical chart|400]](assets/charts/categorical.svg)

## Relationships

- Related: [[Bernoulli]], [[Multinomial]], [[Dirichlet]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Categorical` (C++) and `cydist.categorical` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
