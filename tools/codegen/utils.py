"""Naming and path helpers."""

import re

from codegen.constants import CATEGORY_MAP, RESERVED_NAMES

_RETURN_RE = re.compile(r"\breturn\s+([^;]+);")


def safe_param_name(name: str) -> str:
    if name in RESERVED_NAMES or name == "n":
        return f"{name}_"
    return name


def slug_to_class(vault_id: str) -> str:
    parts = re.sub(r"[^a-z0-9]+", " ", vault_id.lower()).split()
    return "".join(p.capitalize() for p in parts)


def folder_for(category_path: str) -> str:
    return CATEGORY_MAP.get(category_path, "continuous/variable")


def distribution_template_name(cpp_class: str) -> str:
    if cpp_class.endswith("Distribution"):
        return f"{cpp_class}Distribution"
    return f"{cpp_class}Distribution"


def emit_templated_sample_body(sample_body: str) -> str:
    """Rewrite ``return expr;`` (including in ``if`` blocks) to cast through ``Sample``."""
    out: list[str] = []
    for line in sample_body.strip().split("\n"):
        if "return " in line:
            out.append(_RETURN_RE.sub(r"return static_cast<Sample>(\1);", line))
        else:
            out.append(line)
    return "\n        ".join(out)


