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
