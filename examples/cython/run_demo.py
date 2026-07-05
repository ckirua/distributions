#!/usr/bin/env python3
"""Run the Cython demo after building with setup_demo.py."""

import numpy as np

from demo import normal_demo, poisson_demo

N = 8
poisson_out = np.empty(N, dtype=np.int32)
normal_out = np.empty(N, dtype=np.float64)

poisson_demo(poisson_out, mu=4.0, seed=42)
normal_demo(normal_out, mu=0.0, sigma=1.0, seed=42)

print("poisson (mu=4):", poisson_out)
print("normal (0,1):  ", np.round(normal_out, 3))
