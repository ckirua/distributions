"""Parse Wikipedia Template:Probability distributions into taxonomy + registry."""

from __future__ import annotations

import json
import re
from dataclasses import asdict, dataclass
from pathlib import Path

import yaml

from vault_common import META, TEMPLATE_MD, wiki_url, slugify


@dataclass
class DistributionEntry:
    id: str
    name: str
    title: str
    wiki_path: str
    wikipedia: str
    category: str
    category_path: str
    type: str
    support: str
    kind: str = "distribution"


@dataclass
class FamilyEntry:
    id: str
    name: str
    title: str
    wiki_path: str
    wikipedia: str
    kind: str = "family"


TAXONOMY: list[tuple[str, str, str, str, str]] = [
    ("discrete/finite-support", "with finite support", "discrete", "finite", "Discrete univariate"),
    ("discrete/infinite-support", "with infinite support", "discrete", "infinite", "Discrete univariate"),
    (
        "continuous/bounded-interval",
        "supported on a bounded interval",
        "continuous",
        "bounded",
        "Continuous univariate",
    ),
    (
        "continuous/semi-infinite-interval",
        "supported on a semi-infinite",
        "continuous",
        "semi-infinite",
        "Continuous univariate",
    ),
    (
        "continuous/whole-real-line",
        "supported on the whole real line",
        "continuous",
        "real-line",
        "Continuous univariate",
    ),
    (
        "continuous/variable-support",
        "with support whose type varies",
        "continuous",
        "variable",
        "Continuous univariate",
    ),
    ("mixed/continuous-discrete", "continuous-discrete", "mixed", "mixed", "Mixed univariate"),
    ("multivariate/discrete", "_Discrete:_", "multivariate", "discrete", "Multivariate"),
    ("multivariate/continuous", "_Continuous:_", "multivariate", "continuous", "Multivariate"),
    ("multivariate/matrix-valued", "Matrix-valued:", "multivariate", "matrix", "Multivariate"),
    (
        "directional/univariate-circular",
        "_Univariate (circular)",
        "directional",
        "circular",
        "Directional",
    ),
    (
        "directional/bivariate-spherical",
        "_Bivariate (spherical)_",
        "directional",
        "spherical",
        "Directional",
    ),
    (
        "directional/bivariate-toroidal",
        "_Bivariate (toroidal)_",
        "directional",
        "toroidal",
        "Directional",
    ),
    ("directional/multivariate", "_Multivariate_", "directional", "multivariate", "Directional"),
    (
        "degenerate-and-singular/degenerate",
        "_Degenerate_",
        "degenerate",
        "degenerate",
        "Degenerate",
    ),
    (
        "degenerate-and-singular/singular",
        "_Singular_",
        "singular",
        "singular",
        "Degenerate",
    ),
]

FAMILY_NAMES = [
    ("Circular distribution", "Circular_distribution"),
    ("Compound Poisson distribution", "Compound_Poisson_distribution"),
    ("Elliptical distribution", "Elliptical_distribution"),
    ("Exponential family", "Exponential_family"),
    ("Natural exponential family", "Natural_exponential_family"),
    ("Location-scale family", "Location%E2%80%93scale_family"),
    ("Maximum entropy probability distribution", "Maximum_entropy_probability_distribution"),
    ("Mixture distribution", "Mixture_distribution"),
    ("Pearson distribution", "Pearson_distribution"),
    ("Tweedie distribution", "Tweedie_distribution"),
    ("Wrapped distribution", "Wrapped_distribution"),
]


def _extract_links(text: str) -> list[tuple[str, str]]:
    return re.findall(r"\[([^\]]+)\]\(/wiki/([^\"\)]+)", text)


def _split_sections(cell: str, markers: list[str]) -> dict[str, str]:
    positions = [(cell.find(m), m) for m in markers if cell.find(m) >= 0]
    positions.sort()
    sections: dict[str, str] = {}
    for i, (start, marker) in enumerate(positions):
        end = positions[i + 1][0] if i + 1 < len(positions) else len(cell)
        sections[marker] = cell[start:end]
    return sections


def _resolve_ambiguous_name(name: str, wiki_path: str) -> str:
    slug = wiki_path.strip()
    if name == "Negative":
        if "hypergeometric" in slug:
            return "Negative hypergeometric"
        if "multinomial" in slug:
            return "Negative multinomial"
        return "Negative binomial"
    if name == "Generalized":
        if "beta" in slug and "Dirichlet" not in slug:
            return "Generalized beta"
        if "gamma" in slug.lower():
            return "Generalized gamma"
        if "inverse_Gaussian" in slug:
            return "Generalized inverse Gaussian"
        if "Dirichlet" in slug:
            return "Generalized Dirichlet"
        return "Generalized"
    if name == "Inverse":
        if "chi-squared" in slug or "chi-squared" in slug:
            return "Inverse chi-squared"
        if "gamma" in slug:
            return "Inverse gamma"
        if "matrix_gamma" in slug:
            return "Inverse matrix gamma"
        if "Wishart" in slug:
            return "Inverse-Wishart"
        return "Inverse"
    if name == "Noncentral":
        if "chi-squared" in slug:
            return "Noncentral chi-squared"
        if "beta" in slug:
            return "Noncentral beta"
        if "F-distribution" in slug or "F_distribution" in slug:
            return "Noncentral F"
        if "t-distribution" in slug or "t_distribution" in slug:
            return "Noncentral t"
        return "Noncentral"
    if name == "Scaled":
        return "Scaled inverse chi-squared"
    if name == "Discrete" and "Weibull" in slug:
        return "Discrete Weibull"
    if name == "Logarithmic" and "Exponential-logarithmic" in slug:
        return "Exponential-logarithmic"
    if name == "Dirichlet" and "multinomial" in slug:
        return "Dirichlet-multinomial"
    if name == "Normal":
        if "Wishart" in slug:
            return "Normal-Wishart"
        if "inverse-gamma" in slug:
            return "Normal-inverse-gamma"
        if "inverse-Wishart" in slug:
            return "Normal-inverse-Wishart"
        return "Normal"
    return name


def _title_for_name(name: str, wiki_path: str) -> str:
    if name.endswith(" family") or name.endswith(" law"):
        return name[0].upper() + name[1:] if name[0].islower() else name
    if "delta function" in name.lower():
        return "Dirac delta function"
    if name.endswith(" distribution") or name.endswith(" model"):
        return name[0].upper() + name[1:] if name[0].islower() else name
    if name.startswith("Matrix "):
        return f"{name} distribution"
    if name in {"Kent distribution", "Bingham distribution"}:
        return name
    if name.startswith("Wrapped") or name in {"Circular uniform", "Univariate von Mises"}:
        return f"{name} distribution"
    if name == "von Mises-Fisher":
        return "von Mises-Fisher distribution"
    if name == "Bivariate von Mises":
        return "Bivariate von Mises distribution"
    if name == "Cantor":
        return "Cantor distribution"
    if name == "Panjer":
        return "Panjer (a,b,0) class"
    return f"{name} distribution"


def parse_template(text: str) -> tuple[list[DistributionEntry], list[FamilyEntry]]:
    rows = {
        "Discrete univariate": next(l for l in text.splitlines() if l.startswith("| Discrete")),
        "Continuous univariate": next(l for l in text.splitlines() if l.startswith("| Continuous")),
        "Mixed univariate": next(l for l in text.splitlines() if l.startswith("| Mixed")),
        "Multivariate": next(l for l in text.splitlines() if l.startswith("| [Multivariate")),
        "Directional": next(l for l in text.splitlines() if l.startswith("| [Directional")),
        "Degenerate": next(l for l in text.splitlines() if l.startswith("| [Degenerate")),
    }

    by_row: dict[str, list[tuple[str, str, str, str, str]]] = {}
    for item in TAXONOMY:
        by_row.setdefault(item[4], []).append(item)

    distributions: list[DistributionEntry] = []
    seen: set[str] = set()

    for row_name, sections in by_row.items():
        cell = rows[row_name].split("|")[2]
        markers = [s[1] for s in sections]
        split = _split_sections(cell, markers)
        for category_path, marker, dtype, support, _ in sections:
            chunk = split.get(marker, "")
            for raw_name, wiki_path in _extract_links(chunk):
                if raw_name in {"directional", "Directional statistics"}:
                    continue
                name = _resolve_ambiguous_name(raw_name, wiki_path)
                note_id = slugify(name)
                if note_id in seen:
                    note_id = slugify(wiki_path.split("/")[-1].replace("%5F", "_"))
                seen.add(note_id)
                title = _title_for_name(name, wiki_path)
                distributions.append(
                    DistributionEntry(
                        id=note_id,
                        name=name,
                        title=title,
                        wiki_path=wiki_path.strip(),
                        wikipedia=wiki_url(wiki_path.strip()),
                        category=marker.strip("_:"),
                        category_path=category_path,
                        type=dtype,
                        support=support,
                    )
                )

    families = [
        FamilyEntry(
            id=slugify(name.replace(" distribution", "").replace(" family", "")),
            name=name.replace(" distribution", "").replace(" family", ""),
            title=name,
            wiki_path=path,
            wikipedia=wiki_url(path),
        )
        for name, path in FAMILY_NAMES
    ]
    return distributions, families


def write_taxonomy(distributions, families) -> None:
    META.mkdir(parents=True, exist_ok=True)
    payload = {
        "distributions": [asdict(d) for d in distributions],
        "families": [asdict(f) for f in families],
    }
    (META / "taxonomy.json").write_text(
        json.dumps(payload, indent=2, ensure_ascii=False) + "\n",
        encoding="utf-8",
    )


def write_registry(distributions, families) -> None:
    from vault_content import enrich_entry

    registry = {
        "distributions": [enrich_entry(d) for d in distributions],
        "families": [asdict(f) for f in families],
    }
    (META / "registry.yaml").write_text(
        yaml.safe_dump(registry, sort_keys=False, allow_unicode=True),
        encoding="utf-8",
    )


def main() -> None:
    text = TEMPLATE_MD.read_text(encoding="utf-8")
    distributions, families = parse_template(text)
    write_taxonomy(distributions, families)
    print(f"Parsed {len(distributions)} distributions and {len(families)} families")


if __name__ == "__main__":
    main()
