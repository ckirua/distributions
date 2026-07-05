---
id: negative-binomial
title: Negative binomial distribution
aliases:
- Negative binomial
type: discrete
support: infinite
category: discrete/infinite-support
parameters:
- name: target successes
  symbol: r
  role: Larger r shifts mass rightward
- name: success probability
  symbol: p
  role: Controls tail heaviness
wikipedia: https://en.wikipedia.org/wiki/Negative%5Fbinomial%5Fdistribution
chart: assets/charts/negative-binomial.svg
code_status: planned
code_symbols:
  cpp: distributions::NegativeBinomial
  python: cydist.negative_binomial
tags:
- distribution/discrete
- support/infinite
---

> [!summary] In one sentence
> Count of failures before a target number of successes in repeated Bernoulli trials.

## When you meet it

- Trials until the third successful conversion.
- Over-dispersed count data versus Poisson.
- Modeling claim counts with extra variance.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $r$ | target successes | $r>0$ | Larger r shifts mass rightward |
| $p$ | success probability | $(0,1)$ | Controls tail heaviness |

## Core functions

### PMF

$$
P(X=k)=\binom{k+r-1}{k}(1-p)^k p^r
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $r(1-p)/p$ |
| Variance | $r(1-p)/p^2$ |
| Mode | $\lfloor (r-1)(1-p)/p \rfloor$ |

## Shape

Default illustrative parameters are used in the plot below.

![[negative-binomial chart|400]](assets/charts/negative-binomial.svg)

## Relationships

- Related: [[Geometric]], [[Poisson]], [[Gamma]]
- Taxonomy: [[Infinite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::NegativeBinomial` (C++) and `cydist.negative_binomial` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
