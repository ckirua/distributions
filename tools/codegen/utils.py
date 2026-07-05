"""Naming and path helpers."""

import re

from codegen.constants import CATEGORY_MAP, RESERVED_NAMES


def safe_param_name(name: str) -> str:
    if name in RESERVED_NAMES or name == "n":
        return f"{name}_"
    return name


def slug_to_class(vault_id: str) -> str:
    parts = re.sub(r"[^a-z0-9]+", " ", vault_id.lower()).split()
    return "".join(p.capitalize() for p in parts)


def folder_for(category_path: str) -> str:
    return CATEGORY_MAP.get(category_path, "continuous/variable")


