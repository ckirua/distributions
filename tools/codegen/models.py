"""Codegen datamodels."""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class CydistSpec:
    vault_id: str
    py_func: str
    c_func: str
    cpp_class: str
    discrete: bool
    # (param_name, c_decl_type, kind) kind in scalar_int, scalar_double, probs
    params: list[tuple[str, str, str]]
    name: str = ""
    category_path: str = ""
    summary: str = ""
    sampler_tier: str = "family"


@dataclass
class Recipe:
    vault_id: str
    cpp_class: str
    folder: str
    discrete: bool
    includes: list[str] = field(default_factory=list)
    members: list[tuple[str, str, str]] = field(default_factory=list)  # type, name, default
    sample_body: str = ""
    bench_ctor_args: str = ""
    ispc: bool = False
    cydist_params: list[tuple[str, str]] = field(default_factory=list)  # c_type, name
    cydist_skip: bool = False
    sampler_tier: str = "family"  # hand-written | family | heuristic | exact
