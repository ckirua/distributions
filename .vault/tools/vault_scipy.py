"""Scipy distribution mapping for charts and auto-enriched note content."""

from __future__ import annotations

from vault_common import slugify

# vault id -> (scipy.stats name, default kwargs)
SCIPY_SPECS: dict[str, tuple[str, dict]] = {
    "benford": ("benford", {}),
    "bernoulli": ("bernoulli", {"p": 0.35}),
    "beta-binomial": ("betabinom", {"n": 20, "a": 2, "b": 5}),
    "binomial": ("binom", {"n": 20, "p": 0.4}),
    "categorical": ("cat", {"p": [0.5, 0.3, 0.2]}),
    "hypergeometric": ("hypergeom", {"M": 50, "n": 10, "N": 5}),
    "negative-hypergeometric": ("nhypergeom", {"M": 50, "n": 10, "N": 5}),
    "discrete-uniform": ("randint", {"low": 1, "high": 7}),
    "geometric": ("geom", {"p": 0.25}),
    "negative-binomial": ("nbinom", {"n": 5, "p": 0.4}),
    "poisson": ("poisson", {"mu": 4}),
    "skellam": ("skellam", {"mu1": 3, "mu2": 1}),
    "zipf": ("zipfian", {"a": 2.0, "N": 10}),
    "arcsine": ("arcsine", {}),
    "beta": ("beta", {"a": 2, "b": 5}),
    "cauchy": ("cauchy", {"loc": 0, "scale": 1}),
    "chi": ("chi", {"df": 3}),
    "chi-squared": ("chi2", {"df": 3}),
    "erlang": ("erlang", {"a": 2, "scale": 1}),
    "exponential": ("expon", {"scale": 1.0}),
    "f": ("f", {"dfn": 5, "dfd": 10}),
    "gamma": ("gamma", {"a": 2.0, "scale": 2.0}),
    "gumbel": ("gumbel_r", {"loc": 0, "scale": 1}),
    "inverse-gamma": ("invgamma", {"a": 3, "scale": 1}),
    "inverse-gaussian": ("wald", {"mu": 1, "scale": 1}),
    "laplace": ("laplace", {"loc": 0, "scale": 1}),
    "logistic": ("logistic", {"loc": 0, "scale": 1}),
    "log-normal": ("lognorm", {"s": 0.5, "scale": 1}),
    "normal-gaussian": ("norm", {"loc": 0, "scale": 1}),
    "pareto": ("pareto", {"b": 2.0, "scale": 1.0}),
    "rayleigh": ("rayleigh", {"scale": 1.5}),
    "student-t": ("t", {"df": 5}),
    "students-t": ("t", {"df": 5}),
    "weibull": ("weibull_min", {"c": 1.5, "scale": 2.0}),
    "uniform": ("uniform", {"loc": 0, "scale": 1}),
    "triangular": ("triang", {"c": 0.5, "loc": 0, "scale": 1}),
    "wigner-semicircle": ("semicircular", {"loc": -1, "scale": 2}),
    "nakagami": ("nakagami", {"nu": 2, "scale": 1}),
    "maxwell-boltzmann": ("maxwell", {}),
    "lomax": ("lomax", {"c": 2, "scale": 1}),
    "burr": ("burr", {"c": 2, "d": 3, "scale": 1}),
    "dagum": ("dagum", {"c": 2, "d": 3, "scale": 1}),
    "folded-normal": ("foldnorm", {"c": 1, "loc": 0, "scale": 1}),
    "half-normal": ("halfnorm", {"loc": 0, "scale": 1}),
    "half-logistic": ("halflogistic", {"loc": 0, "scale": 1}),
    "reciprocal": ("reciprocal", {"a": 0.5, "b": 2}),
    "raised-cosine": ("cosine", {"loc": 0, "scale": 1}),
    "kumaraswamy": ("kumaraswamy", {"a": 2, "b": 5}),
    "irwin-hall": ("irwinhall", {"n": 4}),
    "von-mises-fisher": ("vonmises_fisher", {"mu": [1, 0, 0], "kappa": 2}),
    "univariate-von-mises": ("vonmises", {"kappa": 2}),
    "wrapped-cauchy": ("wrapcauchy", {"c": 0.5, "loc": 0, "scale": 1}),
    "circular-uniform": ("vonmises", {"kappa": 0}),
    "multinomial": ("multinomial", {"n": 10, "p": [0.4, 0.3, 0.3]}),
    "dirichlet": ("dirichlet", {"alpha": [2, 2, 2]}),
    "wishart": ("wishart", {"df": 5, "scale": [[1, 0.2], [0.2, 1]]}),
    "inverse-wishart": ("invwishart", {"df": 5, "scale": [[1, 0.2], [0.2, 1]]}),
    "matrix-normal": ("matrix_normal", {"mean": [[0, 0]], "rowcov": [[1]], "colcov": [[1]]}),
    "matrix-t": ("matrix_t", {"df": 5, "loc": [[0, 0]], "shape": [[1, 0.2], [0.2, 1]]}),
    "rice": ("rice", {"b": 0.5, "scale": 1.0}),
    "levy": ("levy", {"loc": 0, "scale": 1.0}),
    "gompertz": ("gompertz", {"c": 1, "scale": 1.0}),
    "truncated-normal": ("truncnorm", {"a": -1, "b": 2, "loc": 0, "scale": 1}),
    "log-logistic": ("fisk", {"c": 2, "scale": 1.0}),
    "log-laplace": ("loglaplace", {"c": 1, "scale": 1.0}),
    "generalized-gamma": ("gengamma", {"a": 2, "c": 1, "scale": 1.0}),
    "panjer": ("poisson", {"mu": 4}),
    "conwaymaxwellpoisson": ("poisson", {"mu": 4}),
    "extended-negative-binomial": ("nbinom", {"n": 5, "p": 0.4}),
    "mixed-poisson": ("nbinom", {"n": 2, "p": 0.5}),
    "argus": ("argus", {"chi": 1.0}),
    "beta-rectangular": ("beta", {"a": 2, "b": 5}),
    "generalized-beta": ("beta", {"a": 2, "b": 5}),
    "pert": ("beta", {"a": 3, "b": 3}),
    "frechet": ("invweibull", {"c": 2, "scale": 1.0}),
    "generalized": ("gengamma", {"a": 2, "c": 1, "scale": 1.0}),
    "noncentral-f": ("ncf", {"dfn": 5, "dfd": 10, "nc": 2}),
    "type-2-gumbel": ("weibull_min", {"c": 1.5, "scale": 2.0}),
    "shifted": ("expon", {"loc": 1, "scale": 1.0}),
    "relativistic-breitwigner": ("rel_breitwigner", {"rho": 1.0}),
    "hotellings-t-squared": ("f", {"dfn": 5, "dfd": 10}),
    "asymmetric": ("laplace_asymmetric", {"kappa": 2.0, "loc": 0, "scale": 1.0}),
    "generalized-normal": ("gennorm", {"beta": 1.5, "loc": 0, "scale": 1.0}),
    "generalized-logistic-logistic-beta": ("genlogistic", {"c": 1.0, "loc": 0, "scale": 1.0}),
    "johnsons-su": ("johnsonsu", {"a": 0.5, "b": 1.5, "loc": 0, "scale": 1.0}),
    "noncentral-t": ("nct", {"df": 5, "nc": 2, "loc": 0, "scale": 1.0}),
    "landau": ("landau", {}),
    "stable": ("levy_stable", {"alpha": 1.5, "beta": 0, "loc": 0, "scale": 1.0}),
    "holtsmark": ("levy_stable", {"alpha": 1.5, "beta": 0, "loc": 0, "scale": 1.0}),
    "variance-gamma": ("norminvgauss", {"a": 1.0, "b": 0.0, "loc": 0, "scale": 1.0}),
    "generalized-extreme-value": ("genextreme", {"c": 0.1, "loc": 0, "scale": 1.0}),
    "generalized-pareto": ("genpareto", {"c": 0.1, "loc": 0, "scale": 1.0}),
    "tukey-lambda": ("tukeylambda", {"lam": 0.14}),
}

CONTENT_ALIASES: dict[str, str] = {
    "normal-gaussian": "normal",
    "students-t": "student-t",
    "zipfmandelbrot": "zipf-mandelbrot",
}

PHASE1_ALIASES: dict[str, str] = {
    "normal-gaussian": "normal",
    "zipfmandelbrot": "zipf-mandelbrot",
}


def lookup_scipy_spec(vault_id: str) -> tuple[str, dict] | None:
    if vault_id in SCIPY_SPECS:
        return SCIPY_SPECS[vault_id]
    # fuzzy: strip suffixes
    for key, spec in SCIPY_SPECS.items():
        if vault_id.startswith(key) or key.startswith(vault_id):
            return spec
    guess = vault_id.replace("-distribution", "").replace("-", "")
    for key, spec in SCIPY_SPECS.items():
        if key.replace("-", "") == guess:
            return spec
    return None


def scipy_moments(scipy_name: str, kwargs: dict) -> list[tuple[str, str]]:
    from scipy import stats

    try:
        rv = getattr(stats, scipy_name)(**kwargs)
        mean = rv.mean()
        var = rv.var()
        return [
            ("Mean", f"{mean:.4g}" if abs(mean) < 1e6 else "see docs"),
            ("Variance", f"{var:.4g}" if abs(var) < 1e6 else "see docs"),
        ]
    except Exception:
        return []


def scipy_formula_hint(scipy_name: str, discrete: bool) -> str | None:
    hints = {
        "norm": r"f(x)=\frac{1}{\sigma\sqrt{2\pi}} e^{-(x-\mu)^2/(2\sigma^2)}",
        "binom": r"P(X=k)=\binom{n}{k}p^k(1-p)^{n-k}",
        "poisson": r"P(X=k)=\frac{e^{-\lambda}\lambda^k}{k!}",
        "expon": r"f(x)=\lambda e^{-\lambda x},\ x\ge 0",
        "geom": r"P(X=k)=(1-p)^{k-1}p",
        "nbinom": r"P(X=k)=\binom{k+r-1}{k}(1-p)^k p^r",
        "beta": r"f(x)\propto x^{\alpha-1}(1-x)^{\beta-1}",
        "gamma": r"f(x)\propto x^{k-1}e^{-x/\theta}",
        "cauchy": r"f(x)=\frac{1}{\pi\gamma[1+((x-x_0)/\gamma)^2]}",
        "laplace": r"f(x)=\frac{1}{2b} e^{-|x-\mu|/b}",
        "logistic": r"f(x)=\frac{e^{-(x-\mu)/s}}{s(1+e^{-(x-\mu)/s})^2}",
        "t": r"f(x)\ \text{with}\ \nu\ \text{degrees of freedom (see Wikipedia)}",
        "uniform": r"f(x)=\frac{1}{b-a},\ x\in[a,b]",
        "weibull_min": r"f(x)=\frac{k}{\lambda}\left(\frac{x}{\lambda}\right)^{k-1} e^{-(x/\lambda)^k}",
        "pareto": r"f(x)=\frac{\alpha x_m^\alpha}{x^{\alpha+1}},\ x\ge x_m",
        "chi2": r"f(x)\ \text{with}\ k\ \text{degrees of freedom}",
        "hypergeom": r"P(X=k)=\frac{\binom{K}{k}\binom{N-K}{n-k}}{\binom{N}{n}}",
        "bernoulli": r"P(X=k)=p^k(1-p)^{1-k},\ k\in\{0,1\}",
    }
    if scipy_name in hints:
        return hints[scipy_name]
    return None
