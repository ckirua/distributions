"""Distribution metadata and note body generation."""

from __future__ import annotations

from dataclasses import asdict

import yaml

from vault_common import PHASE1_IDS
from vault_scipy import (
    CONTENT_ALIASES,
    PHASE1_ALIASES,
    lookup_scipy_spec,
    scipy_formula_hint,
    scipy_moments,
)

CONTENT: dict[str, dict] = {
    "bernoulli": {
        "summary": "Models a single yes/no trial with fixed success probability.",
        "scenarios": [
            "A coin flip coded as 1 (heads) or 0 (tails).",
            "Whether a loan defaults in the next period.",
            "Indicator that a machine is up or down.",
        ],
        "parameters": [
            ("p", "success probability", "(0,1)", "Higher p shifts mass toward 1"),
        ],
        "pmf": r"P(X=k)=p^k(1-p)^{1-k},\quad k\in\{0,1\}",
        "moments": [("Mean", r"p"), ("Variance", r"p(1-p)"), ("Mode", r"1 if p>0.5 else 0")],
        "related": ["Bernoulli", "Binomial", "Geometric"],
        "chart_params": {"p": 0.35},
    },
    "binomial": {
        "summary": "Counts successes in a fixed number of independent identical trials.",
        "scenarios": [
            "Number of filled orders out of 20 quotes.",
            "Defect count in a batch of inspected items.",
            "Click-through count from a fixed-size ad sample.",
        ],
        "parameters": [
            ("n", "number of trials", r"n\in\mathbb{N}", "Spreads support from 0 to n"),
            ("p", "success probability", "(0,1)", "Skews the bell-shaped count distribution"),
        ],
        "pmf": r"P(X=k)=\binom{n}{k}p^k(1-p)^{n-k}",
        "moments": [
            ("Mean", r"np"),
            ("Variance", r"np(1-p)"),
            ("Mode", r"\lfloor (n+1)p\rfloor"),
        ],
        "related": ["Bernoulli", "Poisson", "Normal", "Hypergeometric"],
        "chart_params": {"n": 20, "p": 0.4},
    },
    "poisson": {
        "summary": "Models event counts in a fixed interval when events arrive independently at a steady average rate.",
        "scenarios": [
            "Number of trades arriving in one second.",
            "Website visits per minute.",
            "Rare defect arrivals on a production line.",
        ],
        "parameters": [
            ("λ", "rate (expected count)", r"\lambda>0", "Mean and variance both equal λ"),
        ],
        "pmf": r"P(X=k)=\frac{e^{-\lambda}\lambda^k}{k!},\quad k=0,1,2,\ldots",
        "moments": [("Mean", r"\lambda"), ("Variance", r"\lambda"), ("Mode", r"\lfloor\lambda\rfloor")],
        "related": ["Exponential", "Negative binomial", "Binomial"],
        "chart_params": {"mu": 4.0},
    },
    "normal": {
        "summary": "The bell curve: symmetric uncertainty around a central value with controlled spread.",
        "scenarios": [
            "Measurement noise around a true value.",
            "Aggregated returns over many small independent effects.",
            "Heights or test scores in large homogeneous populations.",
        ],
        "parameters": [
            ("μ", "location (mean)", r"\mu\in\mathbb{R}", "Shifts the peak left or right"),
            ("σ", "scale (std. dev.)", r"\sigma>0", "Wider σ = flatter, more dispersed curve"),
        ],
        "pdf": r"f(x)=\frac{1}{\sigma\sqrt{2\pi}}\exp\!\left(-\frac{(x-\mu)^2}{2\sigma^2}\right)",
        "moments": [("Mean", r"\mu"), ("Variance", r"\sigma^2"), ("Mode", r"\mu")],
        "related": ["Log-normal", "Student's t", "Skew normal"],
        "chart_params": {"loc": 0, "scale": 1},
    },
    "exponential": {
        "summary": "Waiting time until the next event when events occur at a constant rate.",
        "scenarios": [
            "Time until the next trade tick.",
            "Lifetime of a component with constant failure rate.",
            "Gap between customer arrivals.",
        ],
        "parameters": [
            ("λ", "rate", r"\lambda>0", "Larger λ → shorter typical waits"),
        ],
        "pdf": r"f(x)=\lambda e^{-\lambda x},\quad x\ge 0",
        "moments": [("Mean", r"1/\lambda"), ("Variance", r"1/\lambda^2"), ("Mode", r"0")],
        "related": ["Poisson", "Gamma", "Weibull"],
        "chart_params": {"scale": 1.0},
    },
    "geometric": {
        "summary": "Number of trials until the first success in repeated independent Bernoulli trials.",
        "scenarios": [
            "Trials until first conversion in a funnel.",
            "Packets sent until first acknowledgement.",
            "Attempts until first successful login.",
        ],
        "parameters": [
            ("p", "success probability", "(0,1]", "Smaller p → longer tail of large counts"),
        ],
        "pmf": r"P(X=k)=(1-p)^{k-1}p,\quad k=1,2,\ldots",
        "moments": [("Mean", r"1/p"), ("Variance", r"(1-p)/p^2"), ("Mode", r"1")],
        "related": ["Bernoulli", "Negative binomial", "Exponential"],
        "chart_params": {"p": 0.25},
    },
    "beta": {
        "summary": "Flexible distribution on the unit interval for proportions, probabilities, and rates bounded between 0 and 1.",
        "scenarios": [
            "Prior belief about a conversion rate.",
            "Random completion percentage.",
            "Probability of default bounded in (0,1).",
        ],
        "parameters": [
            ("α", "shape 1", r"\alpha>0", "With β controls skew on (0,1)"),
            ("β", "shape 2", r"\beta>0", "Equal α=β gives a symmetric hump"),
        ],
        "pdf": r"f(x)=\frac{x^{\alpha-1}(1-x)^{\beta-1}}{B(\alpha,\beta)},\quad x\in(0,1)",
        "moments": [
            ("Mean", r"\alpha/(\alpha+\beta)"),
            ("Variance", r"\alpha\beta/[(\alpha+\beta)^2(\alpha+\beta+1)]"),
            ("Mode", r"(\alpha-1)/(\alpha+\beta-2) if \alpha,\beta>1"),
        ],
        "related": ["Uniform", "Binomial", "Beta-binomial"],
        "chart_params": {"a": 2.0, "b": 5.0},
    },
    "uniform": {
        "summary": "Every value in an interval is equally likely — maximum ignorance on a bounded range.",
        "scenarios": [
            "Random offset in a known window.",
            "Bid shading within a tick band.",
            "Placeholder prior when only bounds are known.",
        ],
        "parameters": [
            ("a", "lower bound", r"a<b", "Left edge of support"),
            ("b", "upper bound", r"a<b", "Right edge of support"),
        ],
        "pdf": r"f(x)=\frac{1}{b-a},\quad x\in[a,b]",
        "moments": [
            ("Mean", r"(a+b)/2"),
            ("Variance", r"(b-a)^2/12"),
            ("Mode", r"any point in [a,b]"),
        ],
        "related": ["Beta", "Triangular"],
        "chart_params": {"loc": 0, "scale": 1},
    },
    "negative-binomial": {
        "summary": "Count of failures before a target number of successes in repeated Bernoulli trials.",
        "scenarios": [
            "Trials until the third successful conversion.",
            "Over-dispersed count data versus Poisson.",
            "Modeling claim counts with extra variance.",
        ],
        "parameters": [
            ("r", "target successes", r"r>0", "Larger r shifts mass rightward"),
            ("p", "success probability", "(0,1)", "Controls tail heaviness"),
        ],
        "pmf": r"P(X=k)=\binom{k+r-1}{k}(1-p)^k p^r",
        "moments": [("Mean", r"r(1-p)/p"), ("Variance", r"r(1-p)/p^2"), ("Mode", r"\lfloor (r-1)(1-p)/p \rfloor")],
        "related": ["Geometric", "Poisson", "Gamma"],
        "chart_params": {"n": 5, "p": 0.4},
    },
    "categorical": {
        "summary": "One draw from finitely many labeled outcomes, generalizing Bernoulli to K categories.",
        "scenarios": [
            "Which product category a customer buys.",
            "Predicted class label from a classifier.",
            "Routing an order to one of several venues.",
        ],
        "parameters": [
            ("p_i", "category probabilities", r"\sum p_i=1", "Higher p_i raises category i mass"),
        ],
        "pmf": r"P(X=i)=p_i",
        "moments": [("Mean", r"— (categorical)"), ("Variance", r"—"), ("Mode", r"\arg\max p_i")],
        "related": ["Bernoulli", "Multinomial", "Dirichlet"],
        "chart_params": {"p": [0.5, 0.3, 0.2]},
    },
    "discrete-uniform": {
        "summary": "Each of finitely many integers is equally likely.",
        "scenarios": [
            "Fair die roll.",
            "Random index into a fixed-size list.",
            "Uninformative choice among labeled buckets.",
        ],
        "parameters": [
            ("a", "lower bound", r"a,b\in\mathbb{Z}", "Smallest possible value"),
            ("b", "upper bound", r"a\le b", "Largest possible value"),
        ],
        "pmf": r"P(X=k)=\frac{1}{b-a+1},\ k=a,\ldots,b",
        "moments": [("Mean", r"(a+b)/2"), ("Variance", r"((b-a+1)^2-1)/12"), ("Mode", r"\text{non-unique}")],
        "related": ["Categorical", "Binomial", "Uniform"],
        "chart_params": {"low": 1, "high": 7},
    },
    "beta-binomial": {
        "summary": "Binomial-like counts when the success probability itself is random (Beta-distributed).",
        "scenarios": [
            "Over-dispersed success counts in heterogeneous populations.",
            "Hierarchical A/B tests with random conversion rates.",
            "Biological counts with extra variance beyond binomial.",
        ],
        "parameters": [
            ("n", "trials", r"n\in\mathbb{N}", "Number of attempts"),
            (r"\alpha,\beta", "Beta shape params", r">0", "Control over-dispersion of p"),
        ],
        "pmf": r"P(X=k)=\binom{n}{k}\frac{B(k+\alpha,n-k+\beta)}{B(\alpha,\beta)}",
        "moments": [("Mean", r"n\alpha/(\alpha+\beta)"), ("Variance", r"see Wikipedia"), ("Mode", r"—")],
        "related": ["Binomial", "Beta", "Negative binomial"],
        "chart_params": {"n": 20, "a": 2, "b": 5},
    },
    "poisson-binomial": {
        "summary": "Sum of independent Bernoulli trials with non-identical success probabilities.",
        "scenarios": [
            "Filled orders when each quote has its own fill probability.",
            "Portfolio defaults with name-specific probabilities.",
            "Network packet success with varying link quality.",
        ],
        "parameters": [
            ("p_i", "trial probabilities", r"p_i\in[0,1]", "Each trial may differ"),
        ],
        "pmf": r"P(X=k)=\sum_{A:\ |A|=k}\prod_{i\in A}p_i\prod_{j\notin A}(1-p_j)",
        "moments": [("Mean", r"\sum p_i"), ("Variance", r"\sum p_i(1-p_i)"), ("Mode", r"—")],
        "related": ["Binomial", "Poisson", "Bernoulli"],
    },
    "zipf": {
        "summary": "Power-law over ranks: common events dominate, rare events trail off slowly.",
        "scenarios": [
            "Word frequency in natural language.",
            "City population ranks.",
            "Web traffic by page popularity.",
        ],
        "parameters": [
            ("s", "exponent", r"s>0", "Larger s → steeper decay"),
            ("N", "support size", r"N\in\mathbb{N}", "Number of ranked categories"),
        ],
        "pmf": r"P(k)\propto k^{-s}",
        "moments": [("Mean", r"depends on s,N"), ("Variance", r"depends on s,N"), ("Mode", r"1")],
        "related": ["Zipf–Mandelbrot", "Yule–Simon"],
        "chart_params": {"a": 2.0, "N": 10},
    },
    "zipf-mandelbrot": {
        "summary": "Rank-frequency law with an offset, flexible tail for ranked discrete data.",
        "scenarios": [
            "Text corpora with head/tail vocabulary.",
            "Popularity rankings with plateau at top ranks.",
            "Citation counts with shifted power law.",
        ],
        "parameters": [
            ("q", "offset", r"q\ge 0", "Shifts mass among top ranks"),
            ("s", "exponent", r"s>0", "Controls tail heaviness"),
        ],
        "pmf": r"P(k)\propto (k+q)^{-s}",
        "moments": [("Mean", r"depends on parameters"), ("Variance", r"depends on parameters"), ("Mode", r"1")],
        "related": ["Zipf", "Zeta"],
    },
    "skellam": {
        "summary": "Difference of two independent Poisson counts.",
        "scenarios": [
            "Net order flow (buys minus sells) in a window.",
            "Goal difference when two Poisson processes compete.",
            "Inventory change from arrivals minus departures.",
        ],
        "parameters": [
            (r"\mu_1", "Poisson rate 1", r">0", "Upward pressure on difference"),
            (r"\mu_2", "Poisson rate 2", r">0", "Downward pressure on difference"),
        ],
        "pmf": r"P(X=k)=e^{-(\mu_1+\mu_2)}(\mu_1/\mu_2)^{k/2}I_{|k|}(2\sqrt{\mu_1\mu_2})",
        "moments": [("Mean", r"\mu_1-\mu_2"), ("Variance", r"\mu_1+\mu_2"), ("Mode", r"\lfloor\mu_1-\mu_2\rfloor")],
        "related": ["Poisson", "Normal (Gaussian)"],
        "chart_params": {"mu1": 3, "mu2": 1},
    },
}

TYPE_SUPPORT_BLURBS = {
    ("discrete", "finite"): "A discrete random variable taking finitely many values.",
    ("discrete", "infinite"): "A discrete random variable on the non-negative integers or similar count set.",
    ("continuous", "bounded"): "A continuous random variable confined to a bounded interval.",
    ("continuous", "semi-infinite"): "A continuous random variable with support on a half-line such as [0,∞).",
    ("continuous", "real-line"): "A continuous random variable with support on the entire real line.",
    ("continuous", "variable"): "A continuous distribution whose support type changes with parameters.",
    ("mixed", "mixed"): "A distribution mixing continuous and discrete components.",
    ("multivariate", "discrete"): "A joint distribution over several discrete random variables.",
    ("multivariate", "continuous"): "A joint distribution over several continuous random variables.",
    ("multivariate", "matrix"): "A distribution over random matrices (covariance-like objects).",
    ("directional", "circular"): "A distribution for angles or directions on the circle.",
    ("directional", "spherical"): "A bivariate distribution on the sphere.",
    ("directional", "toroidal"): "A bivariate distribution on a torus.",
    ("directional", "multivariate"): "A multivariate directional distribution on hyperspheres.",
    ("degenerate", "degenerate"): "A distribution with all probability mass at a single point.",
    ("singular", "singular"): "A singular distribution supported on a fractal or lower-dimensional set.",
}


def _default_content(entry) -> dict:
    key = (entry.type, entry.support)
    blurb = TYPE_SUPPORT_BLURBS.get(key, "A probability distribution used in statistical modeling.")
    return {
        "summary": f"{entry.title}: {blurb}",
        "scenarios": [
            f"Specialized models where {entry.name.lower()} structure appears — see Wikipedia for domain examples.",
            "Building blocks in hierarchical or compound models.",
            "Reference case when comparing against simpler distributions.",
        ],
        "parameters": [],
        "related": ["Probability distribution", "Support and domain"],
    }


def _content_for(entry_id: str) -> dict:
    key = CONTENT_ALIASES.get(entry_id, entry_id)
    if key in CONTENT:
        return CONTENT[key]
    return {}


def _apply_scipy_enrichment(entry, data: dict) -> None:
    spec = lookup_scipy_spec(entry.id)
    if spec is None:
        return
    scipy_name, kwargs = spec
    discrete = entry.type == "discrete" or entry.support in {"finite", "infinite", "discrete"}
    if not data.get("pmf") and not data.get("pdf"):
        hint = scipy_formula_hint(scipy_name, discrete)
        if hint:
            if discrete:
                data["pmf"] = hint
            else:
                data["pdf"] = hint
    if not data.get("moments"):
        data["moments"] = scipy_moments(scipy_name, kwargs)
    data["plottable"] = True
    data["chart_params"] = kwargs


def enrich_entry(entry) -> dict:
    data = asdict(entry)
    custom = _default_content(entry)
    custom.update(_content_for(entry.id))
    _apply_scipy_enrichment(entry, custom)
    plottable = bool(custom.get("chart_params") or lookup_scipy_spec(entry.id))
    data.update(
        {
            "aliases": list(dict.fromkeys([entry.name, entry.title.replace(" distribution", "")])),
            "parameters_meta": custom.get("parameters", []),
            "summary": custom.get("summary", _default_content(entry)["summary"]),
            "scenarios": custom.get("scenarios", _default_content(entry)["scenarios"]),
            "pmf": custom.get("pmf"),
            "pdf": custom.get("pdf"),
            "moments": custom.get("moments", []),
            "related_names": custom.get("related", _default_content(entry)["related"]),
            "chart_params": custom.get("chart_params", {}),
            "chart": f"assets/charts/{entry.id}.svg",
            "plottable": plottable,
        }
    )
    phase_key = PHASE1_ALIASES.get(entry.id, entry.id)
    if phase_key in PHASE1_IDS:
        data["code_status"] = "planned"
        data["code_symbols"] = PHASE1_IDS[phase_key]
    else:
        data["code_status"] = "none"
        data["code_symbols"] = {}
    data["tags"] = [f"distribution/{entry.type}", f"support/{entry.support}"]
    return data


def render_note(entry: dict) -> str:
    params = entry.get("parameters_meta") or []
    param_rows = "\n".join(
        f"| ${sym}$ | {name} | ${domain}$ | {effect} |"
        for sym, name, domain, effect in params
    )
    if not param_rows:
        param_rows = "| — | (see Wikipedia) | — | Parameterization varies by convention |"

    scenarios = "\n".join(f"- {s}" for s in entry.get("scenarios", [])[:3])
    moments = entry.get("moments") or []
    moment_rows = "\n".join(f"| {name} | ${val}$ |" for name, val in moments)
    if not moment_rows:
        moment_rows = "| Mean | See [[Parameters and moments]] |"

    if entry.get("pmf"):
        formula_block = f"### PMF\n\n$$\n{entry['pmf']}\n$$\n"
    elif entry.get("pdf"):
        formula_block = f"### PDF\n\n$$\n{entry['pdf']}\n$$\n"
    else:
        formula_block = (
            f"### Density / mass function\n\n"
            f"> [!note] Formula\n"
            f"> See the [Wikipedia article]({entry['wikipedia']}) for the standard "
            f"parameterization and density or mass function.\n"
        )

    related = entry.get("related_names") or ["Probability distribution", "Support and domain"]
    related_links = ", ".join(f"[[{r}]]" for r in related[:5])

    params_yaml = [
        {"name": p[1], "symbol": p[0], "role": p[3] if len(p) > 3 else p[1]}
        for p in params
    ]
    fm = {
        "id": entry["id"],
        "title": entry["title"],
        "aliases": entry.get("aliases", [entry["name"]]),
        "type": entry["type"],
        "support": entry["support"],
        "category": entry["category_path"],
        "parameters": params_yaml,
        "wikipedia": entry["wikipedia"],
        "chart": f"assets/charts/{entry['id']}.svg",
        "code_status": entry.get("code_status", "none"),
        "code_symbols": entry.get("code_symbols") or {},
        "tags": entry.get("tags", []),
    }
    frontmatter = yaml.safe_dump(fm, sort_keys=False, allow_unicode=True).strip()

    if entry.get("plottable"):
        chart_section = (
            "## Shape\n\nDefault illustrative parameters are used in the plot below.\n\n"
            f"![[{entry['id']} chart|400]](assets/charts/{entry['id']}.svg)\n"
        )
    else:
        chart_section = (
            "## Shape\n\n"
            f"> [!note] Visualization\n"
            f"> No bundled chart for this distribution. See "
            f"[Wikipedia]({entry['wikipedia']}) or [[Support and domain]].\n"
        )

    if entry.get("code_status") == "planned":
        cpp = entry["code_symbols"].get("cpp", "")
        py = entry["code_symbols"].get("python", "")
        code_section = (
            f"Planned for the sampling library. Future symbols: `{cpp}` (C++) and "
            f"`{py}` (Python). See [[Implementation index]] and [[Sampling vs density]]."
        )
    else:
        code_section = "No implementation is planned in the current codebase phase."

    category_label = entry["category_path"].split("/")[-1].replace("-", " ").title()
    return f"""---
{frontmatter}
---

> [!summary] In one sentence
> {entry.get("summary", entry["title"])}

## When you meet it

{scenarios}

## Parameters

| Symbol | Name | Valid range | Effect on shape |
|--------|------|-------------|-----------------|
{param_rows}

## Core functions

{formula_block}
### Moments

| Quantity | Value |
|----------|-------|
{moment_rows}

{chart_section}
## Relationships

- Related: {related_links}
- Taxonomy: [[{category_label}]] · [[Support and domain]] · [[PMF, PDF, and CDF]]

## Implementation

{code_section}

Track status on [[Implementation index]].
"""


def render_family(fam: dict) -> str:
    fm = {
        "id": fam["id"],
        "title": fam["title"],
        "aliases": [fam["name"]],
        "type": "family",
        "wikipedia": fam["wikipedia"],
        "tags": ["family"],
    }
    frontmatter = yaml.safe_dump(fm, sort_keys=False, allow_unicode=True).strip()
    return f"""---
{frontmatter}
---

> [!summary] What is a family?
> A **family** groups distributions that share the same functional form or generating mechanism; members differ mainly by parameters or constraints.

## Idea

Families let you reuse intuition: learn one template, then specialize. See [[Probability distribution]] and [[Parameters and moments]].

## Further reading

- [Wikipedia — {fam['title']}]({fam['wikipedia']})
- [[Special cases and limits]]
- Browse related distributions via the taxonomy MOCs in [[Index]].
"""
