---
id: zipfmandelbrot
title: Zipf–Mandelbrot distribution
aliases:
- Zipf–Mandelbrot
type: discrete
support: finite
category: discrete/finite-support
parameters:
- name: offset
  symbol: q
  role: Shifts mass among top ranks
- name: exponent
  symbol: s
  role: Controls tail heaviness
wikipedia: https://en.wikipedia.org/wiki/Zipf%E2%80%93Mandelbrot%5Flaw
chart: assets/charts/zipfmandelbrot.svg
code_status: planned
code_symbols:
  cpp: distributions::ZipfMandelbrot
  python: cydist.zipf_mandelbrot
tags:
- distribution/discrete
- support/finite
---

> [!summary] In one sentence
> Rank-frequency law with an offset, flexible tail for ranked discrete data.

## When you meet it

- Text corpora with head/tail vocabulary.
- Popularity rankings with plateau at top ranks.
- Citation counts with shifted power law.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| $q$ | offset | $q\ge 0$ | Shifts mass among top ranks |
| $s$ | exponent | $s>0$ | Controls tail heaviness |

## Core functions

### PMF

$$
P(k)\propto (k+q)^{-s}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $depends on parameters$ |
| Variance | $depends on parameters$ |
| Mode | $1$ |

## Shape

Default illustrative parameters are used in the plot below.

![[zipfmandelbrot chart|400]](assets/charts/zipfmandelbrot.svg)

## Relationships

- Related: [[Zipf]], [[Zeta]]
- Taxonomy: [[Finite Support]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

Planned for the sampling library. Future symbols: `distributions::ZipfMandelbrot` (C++) and `cydist.zipf_mandelbrot` (Python). See [[Implementation index]] and [[Sampling vs density]].

Track status on [[Implementation index]].
