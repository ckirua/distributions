#!/usr/bin/env python3
"""Example: plot cydist sample histograms with matplotlib."""

from __future__ import annotations

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402
import numpy as np

from cydist import normal_sample_batch, poisson_sample_batch

N = 50_000
SEED = 42
OUT = Path(__file__).resolve().parent / "poisson_normal_hist.png"

poisson = np.empty(N, dtype=np.int32)
poisson_sample_batch(poisson, mu=4.0, seed=SEED)

normal = np.empty(N, dtype=np.float64)
normal_sample_batch(normal, mu=0.0, sigma=1.0, seed=SEED)

fig, axes = plt.subplots(1, 2, figsize=(10, 4))

axes[0].hist(
    poisson,
    bins=np.arange(-0.5, 15.5, 1),
    density=True,
    color="steelblue",
    edgecolor="white",
)
axes[0].set_title("Poisson(μ=4)")
axes[0].set_xlabel("k")
axes[0].axvline(poisson.mean(), color="crimson", ls="--", label=f"mean={poisson.mean():.2f}")
axes[0].legend()

axes[1].hist(normal, bins=50, density=True, color="seagreen", edgecolor="white")
axes[1].set_title("Normal(0, 1)")
axes[1].set_xlabel("x")
axes[1].axvline(normal.mean(), color="crimson", ls="--", label=f"mean={normal.mean():.3f}")
axes[1].legend()

fig.tight_layout()
fig.savefig(OUT, dpi=120)
print(f"wrote {OUT}")
print(f"poisson mean={poisson.mean():.3f} (expect ~4), normal mean={normal.mean():.3f}")
