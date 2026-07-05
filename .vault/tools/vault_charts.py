#!/usr/bin/env python3
"""Generate SVG charts for plottable distributions."""

from __future__ import annotations

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import yaml
from scipy import stats

from vault_common import VAULT
from vault_scipy import SCIPY_SPECS, lookup_scipy_spec


CHARTS = VAULT / "assets" / "charts"


def _style_axes(ax: plt.Axes, title: str) -> None:
    ax.set_title(title, fontsize=11, pad=8)
    ax.grid(True, alpha=0.25, linewidth=0.6)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def plot_distribution(dist_id: str, scipy_name: str, kwargs: dict, out: Path) -> bool:
    try:
        rv = getattr(stats, scipy_name)(**kwargs)
    except Exception:
        return False

    fig, ax = plt.subplots(figsize=(5, 3.2), dpi=120)
    title = dist_id.replace("-", " ").title()

    if scipy_name == "cat":
        k = np.arange(len(kwargs["p"]))
        ax.bar(k, rv.pmf(k), color="#4C78A8", edgecolor="white", linewidth=0.8)
        ax.set_xlabel("Category")
        ax.set_ylabel("P(X = k)")
    elif scipy_name in {"multinomial", "dirichlet", "wishart", "invwishart", "matrix_normal", "matrix_t", "vonmises_fisher"}:
        plt.close(fig)
        return False
    elif hasattr(rv, "pmf") and scipy_name not in {
        "norm",
        "t",
        "cauchy",
        "laplace",
        "logistic",
        "lognorm",
        "expon",
        "gamma",
        "beta",
        "invgamma",
        "pareto",
        "weibull_min",
        "rayleigh",
        "wald",
        "foldnorm",
        "halfnorm",
        "halflogistic",
        "maxwell",
        "lomax",
        "burr",
        "dagum",
        "cosine",
        "kumaraswamy",
        "semicircular",
        "arcsine",
        "reciprocal",
        "nakagami",
        "wrapcauchy",
        "vonmises",
    }:
        if scipy_name in {"binom", "hypergeom", "randint", "betabinom"}:
            xs = np.arange(int(rv.ppf(0.001)), int(rv.ppf(0.999)) + 1, dtype=int)
        elif scipy_name == "skellam":
            xs = np.arange(-8, 16)
        elif scipy_name in {"zipfian", "zipf"}:
            xs = np.arange(1, kwargs.get("N", 10) + 1)
        else:
            xs = np.arange(0, 25)
        pmf = rv.pmf(xs)
        pmf = np.where(pmf > 0, pmf, np.nan)
        ax.stem(xs, pmf, linefmt="#4C78A8", markerfmt="o", basefmt=" ")
        ax.set_xlabel("k")
        ax.set_ylabel("P(X = k)")
    else:
        xs = np.linspace(rv.ppf(0.001), rv.ppf(0.999), 400)
        ys = rv.pdf(xs)
        ax.plot(xs, ys, color="#4C78A8", linewidth=2)
        ax.fill_between(xs, ys, alpha=0.15, color="#4C78A8")
        ax.set_xlabel("x")
        ax.set_ylabel("f(x)")

    _style_axes(ax, title)
    fig.tight_layout()
    out.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(out, format="svg", bbox_inches="tight")
    plt.close(fig)
    return True


def build_from_registry(registry_path: Path) -> tuple[int, int]:
    registry = yaml.safe_load(registry_path.read_text(encoding="utf-8"))
    ok = skip = 0
    for entry in registry["distributions"]:
        dist_id = entry["id"]
        if not entry.get("plottable"):
            skip += 1
            continue
        spec = lookup_scipy_spec(dist_id)
        if spec is None:
            skip += 1
            continue
        scipy_name, kwargs = spec
        out = CHARTS / f"{dist_id}.svg"
        if plot_distribution(dist_id, scipy_name, kwargs, out):
            ok += 1
        else:
            skip += 1
    return ok, skip


def main() -> None:
    ok, skip = build_from_registry(VAULT / "_meta" / "registry.yaml")
    print(f"Charts: {ok} generated, {skip} skipped")


if __name__ == "__main__":
    main()
