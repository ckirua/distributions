---
id: beta-rectangular
title: Beta rectangular distribution
aliases:
- Beta rectangular
type: continuous
support: bounded
category: continuous/bounded-interval
parameters: []
wikipedia: https://en.wikipedia.org/wiki/Beta%5Frectangular%5Fdistribution
chart: assets/charts/beta-rectangular.svg
code_status: none
code_symbols: {}
tags:
- distribution/continuous
- support/bounded
---

> [!summary] In one sentence
> Beta rectangular distribution: A continuous random variable confined to a bounded interval.

## When you meet it

- Specialized models where beta rectangular structure appears — see Wikipedia for domain examples.
- Building blocks in hierarchical or compound models.
- Reference case when comparing against simpler distributions.

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
| — | (see Wikipedia) | — | Parameterization varies by convention |

## Core functions

### PDF

$$
f(x)\propto x^{\alpha-1}(1-x)^{\beta-1}
$$

### Moments

| Quantity | Value |
|----------|-------|
| Mean | $0.2857$ |
| Variance | $0.02551$ |

## Shape

Default illustrative parameters are used in the plot below.

![[beta-rectangular chart|400]](assets/charts/beta-rectangular.svg)

## Relationships

- Related: [[Probability distribution]], [[Support and domain]]
- Taxonomy: [[Bounded Interval]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

No implementation is planned in the current codebase phase.

Track status on [[Implementation index]].
