"""Tier-B batch_fast hook registry for codegen sample_batch dispatch."""

from __future__ import annotations

from dataclasses import dataclass

from codegen.models import Recipe


@dataclass(frozen=True)
class BatchFastHook:
    """Maps a batch_fast registry id to includes and a Tier-B call."""

    includes: tuple[str, ...]
    call: str  # C++ statement(s); uses member names with trailing underscore


# Wave 1 codegen Tier-B hooks (v0.7.0 batch 2).
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
    "beta": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/beta.hpp",
            "distributions/detail/fast/common.hpp",
        ),
        call=(
            "detail::fast::beta_sample_batch("
            "out, n, alpha_, beta_, detail::batch_seed_from(rng));"
        ),
    ),
    "continuous_bernoulli": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/continuous_bernoulli.hpp",
        ),
        call=(
            "detail::fast::continuous_bernoulli_sample_batch("
            "out, n, lambda_, detail::batch_seed_from(rng));"
        ),
    ),
    "discrete_weibull": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/discrete_weibull.hpp",
        ),
        call=(
            "detail::fast::discrete_weibull_sample_batch("
            "out, n, c_, detail::batch_seed_from(rng));"
        ),
    ),
    "erlang": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/gamma.hpp",
        ),
        call=(
            "detail::fast::gamma_sample_batch("
            "out, n, a_, scale_, detail::batch_seed_from(rng));"
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
    "gamma": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/gamma.hpp",
        ),
        call=(
            "detail::fast::gamma_sample_batch("
            "out, n, shape_, scale_, detail::batch_seed_from(rng));"
        ),
    ),
    "log_normal": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/log_normal.hpp",
        ),
        call=(
            "detail::fast::log_normal_sample_batch("
            "out, n, mu_, sigma_, detail::batch_seed_from(rng));"
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
    "poisson": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/poisson.hpp",
        ),
        call=(
            "detail::fast::poisson_sample_batch("
            "out, n, mu_, detail::batch_seed_from(rng));"
        ),
    ),
    "student_t": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/student_t.hpp",
        ),
        call=(
            "detail::fast::student_t_sample_batch("
            "out, n, df_, detail::batch_seed_from(rng));"
        ),
    ),
    "uniform": BatchFastHook(
        includes=(
            "distributions/detail/counter_rng.hpp",
            "distributions/detail/fast/common.hpp",
            "distributions/detail/fast/uniform.hpp",
        ),
        call=(
            "detail::fast::bounded_uniform_sample_batch("
            "out, n, lo_, hi_, detail::batch_seed_from(rng));"
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
    loop = (
        "for (std::size_t i = 0; i < n; ++i) {\n"
        "            out[i] = sample(rng);\n"
        "        }"
    )
    hook = resolve_batch_fast(recipe)
    if hook is None:
        return (
            f"""void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {{
        {loop}
    }}""",
            (),
        )
    fast_path = f"""if constexpr (std::is_same_v<Sample, double>) {{
            if (n >= detail::kFastThreshold) {{
                {hook.call}
                return;
            }}
        }}"""
    return (
        f"""void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {{
        {fast_path}
        {loop}
    }}""",
        hook.includes,
    )
