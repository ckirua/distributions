#!/usr/bin/env python3
"""Example: Python API — pre-allocated NumPy buffers, in-place batch sampling."""

from __future__ import annotations

import numpy as np

from cydist import (
    normal_sample_batch,
    poisson_sample_batch,
)

N = 8
SEED = 42

# Discrete: int32 buffer
poisson_out = np.empty(N, dtype=np.int32)
poisson_sample_batch(poisson_out, mu=4.0, seed=SEED)
print("poisson (mu=4):", poisson_out)

# Continuous: float64 buffer
normal_out = np.empty(N, dtype=np.float64)
normal_sample_batch(normal_out, mu=0.0, sigma=1.0, seed=SEED)
print("normal (0,1):  ", np.round(normal_out, 3))

# Quick sanity check on a larger batch
big = np.empty(100_000, dtype=np.int32)
poisson_sample_batch(big, mu=4.0, seed=SEED)
print(f"poisson mean over 100k: {big.mean():.3f} (expect ~4)")
