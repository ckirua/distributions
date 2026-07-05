---
id: normal-gaussian
title: Normal (Gaussian) distribution
aliases:
- Normal (Gaussian)
type: continuous
support: real-line
category: continuous/whole-real-line
parameters:
- name: location (mean)
  symbol: μ
  role: Shifts the peak left or right
- name: scale (std. dev.)
  symbol: σ
  role: Wider σ = flatter, more dispersed curve
wikipedia: https://en.wikipedia.org/wiki/Normal%5Fdistribution
chart: assets/charts/normal-gaussian.svg
code_status: planned
code_symbols:
  cpp: distributions::Normal
  python: cydist.normal
tags:
- distribution/continuous
- support/real-line
---

> [!summary] In one sentence
> The bell curve: symmetric uncertainty around a central value with controlled spread.

## When you meet it

- Measurement noise around a true value.
- Aggregated returns over many small independent effects.
- Heights or test scores in large homogeneous populations.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $μ$ | location (mean) | $\mu\in\mathbb{R}$ | Shifts the peak left or right |
| $σ$ | scale (std. dev.) | $\sigma>0$ | Wider σ = flatter, more dispersed curve |

## Core functions

### PDF

$$
f(x)=\frac{1}{\sigma\sqrt{2\pi}}\exp\!\left(-\frac{(x-\mu)^2}{2\sigma^2}\right)
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $\mu$ |
| Variance | $\sigma^2$ |
| Mode | $\mu$ |

## Shape

Default illustrative parameters are used in the plot below.

![[normal-gaussian chart|400]](assets/charts/normal-gaussian.svg)

## Relationships

- Related: [[Log-normal]], [[Student's t]], [[Skew normal]]
- Taxonomy: [[Whole Real Line]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::Normal` (C++) and `cydist.normal` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
