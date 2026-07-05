"""Infer debug validate_body for codegen distribution recipes."""

from __future__ import annotations

from codegen.models import Recipe

# Explicit checks where member names alone are ambiguous.
_VAULT_VALIDATE: dict[str, str] = {
    "generalized-hyperbolic": (
        "detail::assert_finite(p_);\n"
        "        detail::assert_gh_support(a_, b_);"
    ),
    "hypergeometric": (
        "detail::assert_nonnegative_int(M_);\n"
        "        detail::assert_nonnegative_int(n_success_);\n"
        "        detail::assert_nonnegative_int(N_draws_);\n"
        "        assert(n_success_ <= M_);\n"
        "        assert(N_draws_ <= M_);"
    ),
    "negative-hypergeometric": (
        "detail::assert_nonnegative_int(M_);\n"
        "        detail::assert_nonnegative_int(n_);\n"
        "        detail::assert_nonnegative_int(N_);\n"
        "        assert(n_ <= M_);\n"
        "        assert(N_ <= M_);"
    ),
    "zeta": (
        "detail::assert_positive_int(N_);\n"
        "        detail::assert_strictly_positive(s_);"
    ),
    "kumaraswamy": (
        "detail::assert_strictly_positive(a_);\n"
        "        detail::assert_strictly_positive(b_);"
    ),
    "reciprocal": (
        "detail::assert_strictly_positive(a_);\n"
        "        detail::assert_strictly_positive(b_);\n"
        "        detail::assert_double_interval(a_, b_);"
    ),
    "u-quadratic": "detail::assert_double_interval(a_, b_);",
    "discrete-phase-type": (
        "detail::assert_probability(p1_);\n"
        "        detail::assert_probability(p2_);"
    ),
    "delaporte": (
        "detail::assert_nonnegative(lambda_);\n"
        "        detail::assert_strictly_positive(r_);\n"
        "        detail::assert_probability(p_);"
    ),
    "extended-negative-binomial": (
        "detail::assert_strictly_positive(r_);\n"
        "        detail::assert_open_unit_interval(p_);"
    ),
    "logarithmic": "detail::assert_open_unit_interval(p_);",
    "floryschulz": "detail::assert_open_unit_interval(p_);",
}

# ``a`` and ``b`` are interval endpoints (require a < b), not shape parameters.
_INTERVAL_AB_VAULTS = frozenset({"u-quadratic"})

_PROBABILITY_NAMES = frozenset({"p", "p1", "p2", "prob", "weight", "w1"})
_STRICT_POSITIVE_NAMES = frozenset({
    "scale",
    "rate",
    "sigma",
    "shape",
    "df",
    "dfn",
    "dfd",
    "nu",
    "chi",
    "kappa",
    "kappa1",
    "kappa2",
    "s",
    "rho",
    "f",
    "alpha",
    "beta",
    "gamma",
    "r",
    "lam",
    "c",
    "a",
    "b",
    "q",
})
_NONNEGATIVE_NAMES = frozenset({
    "mu",
    "mu1",
    "mu2",
    "lambda",
    "lambda1",
    "lambda2",
    "ncp",
    "nc",
    "kappa",
    "kappa1",
    "kappa2",
})
_FINITE_NAMES = frozenset({"loc", "mode", "x0", "mu", "p"})
_POSITIVE_INT_NAMES = frozenset({"N", "n_max"})
_NONNEGATIVE_INT_NAMES = frozenset({"M", "n", "n_success", "N_draws", "trials"})


def _member_names(members: list[tuple[str, str, str]]) -> set[str]:
    return {name for _, name, _ in members}


def _infer_member_line(ctype: str, name: str, names: set[str], vault_id: str) -> str | None:
    member = f"{name}_"
    if ctype == "int":
        if name in _POSITIVE_INT_NAMES:
            return f"detail::assert_positive_int({member});"
        if name in _NONNEGATIVE_INT_NAMES:
            return f"detail::assert_nonnegative_int({member});"
        return f"detail::assert_nonnegative_int({member});"

    if name in _PROBABILITY_NAMES:
        return f"detail::assert_probability({member});"

    if name in {"lo", "low", "hi", "high"}:
        return None

    if name in _STRICT_POSITIVE_NAMES:
        if name in {"a", "b"} and vault_id in _INTERVAL_AB_VAULTS:
            return None
        if name in {"a", "b"} and {"lo", "hi", "low", "high"}.intersection(names):
            return None
        if name == "kappa" or name.startswith("kappa"):
            return f"detail::assert_nonnegative({member});"
        if name == "q" and vault_id.endswith("mandelbrot"):
            return f"detail::assert_zipf_mandelbrot_q({member});"
        if name in {"mu", "mu1", "mu2"} and "sigma" in names:
            return f"detail::assert_finite({member});"
        if name in {"mu", "mu1", "mu2"}:
            return f"detail::assert_nonnegative({member});"
        if name == "lambda" or name.startswith("lambda"):
            return f"detail::assert_nonnegative({member});"
        return f"detail::assert_strictly_positive({member});"

    if name in _NONNEGATIVE_NAMES:
        if name in {"mu", "mu1", "mu2"} and "sigma" in names:
            return f"detail::assert_finite({member});"
        return f"detail::assert_nonnegative({member});"

    if name in _FINITE_NAMES:
        if name == "p" and vault_id == "generalized-hyperbolic":
            return f"detail::assert_finite({member});"
        if name == "mu" and "sigma" in names:
            return f"detail::assert_finite({member});"
        return f"detail::assert_finite({member});"

    return f"detail::assert_finite({member});"


def _infer_pair_lines(members: list[tuple[str, str, str]], vault_id: str) -> list[str]:
    names = _member_names(members)
    lines: list[str] = []

    if "lo" in names and "hi" in names:
        lines.append("detail::assert_double_interval(lo_, hi_);")
    if "low" in names and "high" in names:
        lines.append("detail::assert_double_interval(low_, high_);")

    if (
        "a" in names
        and "b" in names
        and vault_id not in _VAULT_VALIDATE
        and not {"lo", "hi", "low", "high"}.intersection(names)
        and vault_id in _INTERVAL_AB_VAULTS
    ):
        lines.append("detail::assert_double_interval(a_, b_);")

    if "lo" in names and "mode" in names and "hi" in names:
        lines.append("detail::assert_double_interval(lo_, hi_);")

    return lines


def infer_validate_body(vault_id: str, members: list[tuple[str, str, str]]) -> str:
    if not members:
        return ""
    if vault_id in _VAULT_VALIDATE:
        return _VAULT_VALIDATE[vault_id]

    names = _member_names(members)
    lines: list[str] = []
    seen: set[str] = set()

    for ctype, name, _default in members:
        line = _infer_member_line(ctype, name, names, vault_id)
        if line and line not in seen:
            lines.append(line)
            seen.add(line)

    for line in _infer_pair_lines(members, vault_id):
        if line not in seen:
            lines.append(line)
            seen.add(line)

    return "\n        ".join(lines)


def finalize_recipe_validation(recipes: dict[str, Recipe]) -> None:
    """Fill ``validate_body`` on codegen recipes that do not set it explicitly."""
    for recipe in recipes.values():
        if recipe.validate_body or not recipe.members:
            continue
        recipe.validate_body = infer_validate_body(recipe.vault_id, recipe.members)


# --- cydist Python ValueError (batch 3) ---

ParamSpec = tuple[str, str, str]  # name, c_type, kind (scalar_int | scalar_double | probs)


def _py_var(name: str) -> str:
    from codegen.utils import safe_param_name

    return safe_param_name(name)


def _params_to_members(params: list[ParamSpec]) -> list[tuple[str, str, str]]:
    members: list[tuple[str, str, str]] = []
    for name, c_type, kind in params:
        if kind == "probs":
            continue
        ctype = "int" if kind == "scalar_int" else "double"
        members.append((ctype, name, ""))
    return members


_VAULT_PYTHON_VALIDATE: dict[str, list[str]] = {
    "generalized-hyperbolic": [
        '_check_finite({p}, "p")',
        "if not ({a} > abs({b})): raise ValueError('generalized hyperbolic requires a > |b|')",
    ],
    "hypergeometric": [
        '_check_nonneg_int({M}, "M")',
        '_check_nonneg_int({n_success}, "n_success")',
        '_check_nonneg_int({N_draws}, "N_draws")',
        "if {n_success} > {M}: raise ValueError('requires n_success <= M')",
        "if {N_draws} > {M}: raise ValueError('requires N_draws <= M')",
    ],
    "negative-hypergeometric": [
        '_check_nonneg_int({M}, "M")',
        '_check_nonneg_int({n}, "n")',
        '_check_nonneg_int({N}, "N")',
        "if {n} > {M}: raise ValueError('requires n <= M')",
        "if {N} > {M}: raise ValueError('requires N <= M')",
    ],
    "zeta": [
        '_check_positive_int({N}, "N")',
        '_check_positive({s}, "s")',
    ],
    "kumaraswamy": [
        '_check_positive({a}, "a")',
        '_check_positive({b}, "b")',
    ],
    "reciprocal": [
        '_check_positive({a}, "a")',
        '_check_positive({b}, "b")',
        '_check_double_interval({a}, {b})',
    ],
    "u-quadratic": ["_check_double_interval({a}, {b})"],
    "discrete-phase-type": [
        '_check_probability({p1}, "p1")',
        '_check_probability({p2}, "p2")',
    ],
    "delaporte": [
        '_check_nonneg({lambda_}, "lambda")',
        '_check_positive({r}, "r")',
        '_check_probability({p}, "p")',
    ],
    "extended-negative-binomial": [
        '_check_positive({r}, "r")',
        '_check_open_unit({p}, "p")',
    ],
    "logarithmic": ['_check_open_unit({p}, "p")'],
    "floryschulz": ['_check_open_unit({p}, "p")'],
    "discrete-uniform": ["if {low} > {high}: raise ValueError('requires low <= high')"],
    "zipfmandelbrot": [
        '_check_positive_int({N}, "N")',
        '_check_positive({s}, "s")',
        "if {q} <= -1.0: raise ValueError('zipf mandelbrot requires q > -1')",
    ],
}


def _format_py_checks(template_lines: list[str], params: list[ParamSpec]) -> list[str]:
    fmt: dict[str, str] = {_py_var(name): _py_var(name) for name, _, _ in params}
    # also allow bare names in templates
    for name, _, _ in params:
        fmt[name] = _py_var(name)
    return [line.format(**fmt) for line in template_lines]


def _infer_cydist_member_check(
    ctype: str, name: str, names: set[str], vault_id: str, var: str
) -> str | None:
    if ctype == "int":
        if name in _POSITIVE_INT_NAMES:
            return f'_check_positive_int({var}, "{name}")'
        return f'_check_nonneg_int({var}, "{name}")'

    if name == "p" and vault_id in {
        "geometric",
        "logarithmic",
        "floryschulz",
        "extended-negative-binomial",
        "negative-binomial",
    }:
        return f'_check_open_unit({var}, "{name}")'
    if name == "r" and vault_id == "negative-binomial" and ctype == "int":
        return f'_check_positive_int({var}, "r")'

    if name in _PROBABILITY_NAMES:
        return f'_check_probability({var}, "{name}")'

    if name in {"lo", "low", "hi", "high"}:
        return None

    if name in _STRICT_POSITIVE_NAMES:
        if name in {"a", "b"} and vault_id in _INTERVAL_AB_VAULTS:
            return None
        if name in {"a", "b"} and {"lo", "hi", "low", "high"}.intersection(names):
            return None
        if name == "kappa" or name.startswith("kappa"):
            return f'_check_nonneg({var}, "{name}")'
        if name == "q" and "mandelbrot" in vault_id:
            return f"if {var} <= -1.0: raise ValueError('{name} must be > -1')"
        if name in {"mu", "mu1", "mu2"} and "sigma" in names:
            return f'_check_finite({var}, "{name}")'
        if name in {"mu", "mu1", "mu2"}:
            return f'_check_nonneg({var}, "{name}")'
        if name == "lambda" or name.startswith("lambda"):
            return f'_check_nonneg({var}, "{name}")'
        return f'_check_positive({var}, "{name}")'

    if name in _NONNEGATIVE_NAMES:
        if name in {"mu", "mu1", "mu2"} and "sigma" in names:
            return f'_check_finite({var}, "{name}")'
        return f'_check_nonneg({var}, "{name}")'

    if name in _FINITE_NAMES:
        return f'_check_finite({var}, "{name}")'

    return f'_check_finite({var}, "{name}")'


def _infer_cydist_pair_checks(members: list[tuple[str, str, str]], vault_id: str) -> list[str]:
    names = _member_names(members)
    lines: list[str] = []
    if "lo" in names and "hi" in names:
        lines.append("_check_double_interval({lo}, {hi})".format(lo=_py_var("lo"), hi=_py_var("hi")))
    if "low" in names and "high" in names:
        lines.append(
            "_check_double_interval({low}, {high})".format(low=_py_var("low"), high=_py_var("high"))
        )
    if (
        "a" in names
        and "b" in names
        and vault_id not in _VAULT_PYTHON_VALIDATE
        and vault_id not in _VAULT_VALIDATE
        and vault_id in _INTERVAL_AB_VAULTS
    ):
        lines.append("_check_double_interval({a}, {b})".format(a=_py_var("a"), b=_py_var("b")))
    return lines


def infer_cydist_python_checks(vault_id: str, params: list[ParamSpec]) -> list[str]:
    if not params:
        return []

    for name, _c_type, kind in params:
        if kind == "probs":
            return ["_check_probs(probs)"]

    if vault_id in _VAULT_PYTHON_VALIDATE:
        return _format_py_checks(_VAULT_PYTHON_VALIDATE[vault_id], params)

    if vault_id in _VAULT_VALIDATE:
        # Map vault overrides that share names with cydist params.
        members = _params_to_members(params)
        names = _member_names(members)
        lines: list[str] = []
        seen: set[str] = set()
        for ctype, name, _ in members:
            var = _py_var(name)
            line = _infer_cydist_member_check(ctype, name, names, vault_id, var)
            if line and line not in seen:
                lines.append(line)
                seen.add(line)
        for line in _infer_cydist_pair_checks(members, vault_id):
            if line not in seen:
                lines.append(line)
        return lines

    members = _params_to_members(params)
    names = _member_names(members)
    lines = []
    seen: set[str] = set()
    for ctype, name, _ in members:
        var = _py_var(name)
        line = _infer_cydist_member_check(ctype, name, names, vault_id, var)
        if line and line not in seen:
            lines.append(line)
            seen.add(line)
    for line in _infer_cydist_pair_checks(members, vault_id):
        if line not in seen:
            lines.append(line)
    return lines


CYDIST_PYVALIDATE_HELPERS = '''
import numpy as np


def _check_probability(x, name):
    if x < 0.0 or x > 1.0:
        raise ValueError(f"{name} must be in [0, 1], got {x}")


def _check_open_unit(x, name):
    if x <= 0.0 or x > 1.0:
        raise ValueError(f"{name} must be in (0, 1], got {x}")


def _check_positive(x, name):
    if x <= 0.0:
        raise ValueError(f"{name} must be positive, got {x}")


def _check_nonneg(x, name):
    if x < 0.0:
        raise ValueError(f"{name} must be non-negative, got {x}")


def _check_finite(x, name):
    if not np.isfinite(x):
        raise ValueError(f"{name} must be finite, got {x}")


def _check_positive_int(x, name):
    if x <= 0:
        raise ValueError(f"{name} must be positive, got {x}")


def _check_nonneg_int(x, name):
    if x < 0:
        raise ValueError(f"{name} must be non-negative, got {x}")


def _check_double_interval(lo, hi):
    if lo >= hi:
        raise ValueError(f"requires lo < hi, got {lo} >= {hi}")


def _check_probs(probs):
    if probs.shape[0] == 0:
        raise ValueError("probs must be non-empty")
    arr = np.asarray(probs, dtype=np.float64)
    if np.any(arr < 0.0):
        raise ValueError("probs must be non-negative")
'''

