#!/usr/bin/env python3
"""Example: fill a polars DataFrame with cydist batch samples."""

from __future__ import annotations

import numpy as np
import polars as pl

from cydist import geometric_sample_batch, poisson_sample_batch

N = 10_000
SEED = 42

poisson = np.empty(N, dtype=np.int32)
poisson_sample_batch(poisson, mu=3.5, seed=SEED)

geometric = np.empty(N, dtype=np.int32)
geometric_sample_batch(geometric, p=0.25, seed=SEED)

df = pl.DataFrame({"poisson": poisson, "geometric": geometric})

print("describe():")
print(df.describe())
print("\nhead():")
print(df.head())
