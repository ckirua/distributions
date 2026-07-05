#!/usr/bin/env python3
"""Example: fill a pandas DataFrame with cydist batch samples."""

from __future__ import annotations

import numpy as np
import pandas as pd

from cydist import bernoulli_sample_batch, exponential_sample_batch, normal_sample_batch

N = 10_000
SEED = 42

bernoulli = np.empty(N, dtype=np.int32)
bernoulli_sample_batch(bernoulli, p=0.35, seed=SEED)

exponential = np.empty(N, dtype=np.float64)
exponential_sample_batch(exponential, rate=1.0, seed=SEED)

normal = np.empty(N, dtype=np.float64)
normal_sample_batch(normal, mu=2.0, sigma=0.5, seed=SEED)

df = pd.DataFrame(
    {
        "bernoulli": bernoulli,
        "exponential": exponential,
        "normal": normal,
    }
)

print("describe():")
print(df.describe().round(3).to_string())
print("\nhead():")
print(df.head().to_string(index=False))
