"""Tier-B batch_fast hook registry for codegen sample_batch dispatch."""

from __future__ import annotations

from dataclasses import dataclass

from codegen.models import Recipe


@dataclass(frozen=True)
class BatchFastHook:
    """Maps a batch_fast registry id to includes and a Tier-B call."""

    includes: tuple[str, ...]
    call: str  # C++ statement(s); uses member names with trailing underscore


# Known hooks. Wave 1 ids (poisson, gamma, beta, uniform, students-t) land in batch 2.
BATCH_FAST_HOOKS: dict[str, BatchFastHook] = {
    "bernoulli": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/bernoulli.hpp",
            "distributions/detail/fast/common.hpp",
        ),
        call=(
            "detail::fast::bernoulli_sample_batch("
            "out, n, p_, detail::batch_seed_from(rng));"
        ),
    ),
    "exponential": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/exponential.hpp",
        ),
        call=(
            "detail::fast::exponential_sample_batch("
            "out, n, rate_, detail::batch_seed_from(rng));"
        ),
    ),
    "normal": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/normal.hpp",
        ),
        call=(
            "detail::fast::normal_sample_batch("
            "out, n, mu_, sigma_, detail::batch_seed_from(rng));"
        ),
    ),
}


def resolve_batch_fast(recipe: Recipe) -> BatchFastHook | None:
    if not recipe.batch_fast:
        return None
    hook = BATCH_FAST_HOOKS.get(recipe.batch_fast)
    if hook is None:
        known = ", ".join(sorted(BATCH_FAST_HOOKS))
        raise ValueError(
            f"unknown batch_fast {recipe.batch_fast!r} for {recipe.vault_id!r}; "
            f"known hooks: {known or '(none)'}"
        )
    return hook


def emit_sample_batch(recipe: Recipe) -> tuple[str, tuple[str, ...]]:
    """Return (sample_batch method body, extra includes)."""
    out_type = "int" if recipe.discrete else "double"
    loop = (
        "for (std::size_t i = 0; i < n; ++i) {\n"
        "            out[i] = sample(rng);\n"
        "        }"
    )
    hook = resolve_batch_fast(recipe)
    if hook is None:
        return (
            f"""void sample_batch({out_type}* out, std::size_t n, Pcg32& rng) const {{
        {loop}
    }}""",
            (),
        )
    return (
        f"""void sample_batch({out_type}* out, std::size_t n, Pcg32& rng) const {{
        if (n >= detail::kFastThreshold) {{
            {hook.call}
            return;
        }}
        {loop}
    }}""",
        hook.includes,
    )
