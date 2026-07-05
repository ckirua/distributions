"""Codegen template emission (Sample type parameter on generated structs)."""

from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from codegen.emit import emit_header  # noqa: E402
from codegen.models import Recipe  # noqa: E402
from codegen.utils import distribution_template_name, emit_templated_sample_body  # noqa: E402


def test_distribution_template_name() -> None:
    assert distribution_template_name("Gamma") == "GammaDistribution"
    assert distribution_template_name("FDistribution") == "FDistributionDistribution"


def test_emit_templated_sample_body_casts_returns() -> None:
    body = emit_templated_sample_body(
        "const double u = rng.next_double();\n"
        "        return scale_ * u;"
    )
    assert "static_cast<Sample>" in body


def test_codegen_continuous_header_is_templated() -> None:
    recipe = Recipe(
        vault_id="gamma",
        cpp_class="Gamma",
        folder="continuous/semi_infinite",
        discrete=False,
        members=[("double", "shape", "2.0"), ("double", "scale", "2.0")],
        sample_body="return detail::sample_gamma(rng, shape_, scale_);",
        batch_fast="gamma",
    )
    header = emit_header(recipe)
    assert "template <typename Sample = double>" in header
    assert "struct GammaDistribution" in header
    assert "using Gamma = GammaDistribution<double>;" in header
    assert "void sample_batch(Sample* out" in header
    assert "if constexpr (std::is_same_v<Sample, double>)" in header


def test_codegen_discrete_header_is_templated() -> None:
    recipe = Recipe(
        vault_id="poisson",
        cpp_class="Poisson",
        folder="discrete/infinite",
        discrete=True,
        members=[("double", "mu", "4.0")],
        sample_body="return detail::sample_poisson(rng, mu_);",
    )
    header = emit_header(recipe)
    assert "template <typename Sample = int>" in header
    assert "static_assert(is_discrete_sample_v<Sample>);" in header
    assert "using Poisson = PoissonDistribution<int>;" in header
