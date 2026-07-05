---
id: students-t
title: Student's _t_ distribution
aliases:
- Student's _t_
type: continuous
support: real-line
category: continuous/whole-real-line
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Student%27s%5Ft-distribution
chart: assets/charts/students-t.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/real-line
---

> [!summary] In one sentence
> Student's _t_ distribution: A continuous random variable with support on the entire real line.

## When you meet it

- Specialized models where student's _t_ structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)\ \text{with}\ \nu\ \text{degrees of freedom (see Wikipedia)}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $0$ |
| Variance | $1.667$ |

## Shape

Default illustrative parameters are used in the plot below.

![[students-t chart|400]](assets/charts/students-t.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Whole Real Line]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
