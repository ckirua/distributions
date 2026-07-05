"""Shared paths and helpers for vault build tools."""

from __future__ import annotations

import re
import unicodedata
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]  # .vault/
VAULT = ROOT
META = VAULT / "_meta"
TEMPLATE_MD = META / "Template_Probability_distributions.md"

PHASE1_IDS: dict[str, dict[str, str]] = {
    "bernoulli": {"cpp": "distributions::Bernoulli", "python": "cydist.bernoulli"},
    "discrete-uniform": {
        "cpp": "distributions::DiscreteUniform",
        "python": "cydist.discrete_uniform",
    },
    "binomial": {"cpp": "distributions::Binomial", "python": "cydist.binomial"},
    "geometric": {"cpp": "distributions::Geometric", "python": "cydist.geometric"},
    "exponential": {
        "cpp": "distributions::Exponential",
        "python": "cydist.exponential",
    },
    "normal": {"cpp": "distributions::Normal", "python": "cydist.normal"},
    "normal-gaussian": {"cpp": "distributions::Normal", "python": "cydist.normal"},
    "poisson-binomial": {
        "cpp": "distributions::PoissonBinomial",
        "python": "cydist.poisson_binomial",
    },
    "beta-binomial": {
        "cpp": "distributions::BetaBinomial",
        "python": "cydist.beta_binomial",
    },
    "categorical": {
        "cpp": "distributions::Categorical",
        "python": "cydist.categorical",
    },
    "negative-binomial": {
        "cpp": "distributions::NegativeBinomial",
        "python": "cydist.negative_binomial",
    },
    "zipf": {"cpp": "distributions::Zipf", "python": "cydist.zipf"},
    "zipf-mandelbrot": {
        "cpp": "distributions::ZipfMandelbrot",
        "python": "cydist.zipf_mandelbrot",
    },
    "zipfmandelbrot": {
        "cpp": "distributions::ZipfMandelbrot",
        "python": "cydist.zipf_mandelbrot",
    },
    "skellam": {"cpp": "distributions::Skellam", "python": "cydist.skellam"},
}


def slugify(name: str) -> str:
    text = unicodedata.normalize("NFKD", name)
    text = text.encode("ascii", "ignore").decode("ascii")
    text = text.lower()
    text = re.sub(r"['']s\b", "s", text)
    text = re.sub(r"[^a-z0-9]+", "-", text)
    return text.strip("-")


def wiki_url(path: str) -> str:
    clean = path.strip().rstrip()
    return f"https://en.wikipedia.org/wiki/{clean}"


def note_title(name: str) -> str:
    if name.endswith(" distribution") or name.endswith(" law") or name.endswith(" family"):
        return name[0].upper() + name[1:]
    if "function" in name.lower():
        return name[0].upper() + name[1:]
    return f"{name} distribution"


def note_path(category_path: str, note_id: str) -> Path:
    return VAULT / "03-Distributions" / category_path / f"{note_id}.md"


def family_note_path(note_id: str) -> Path:
    return VAULT / "02-Families" / f"{note_id}.md"
