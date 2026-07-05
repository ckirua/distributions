"""Shared constants and paths for library codegen."""

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VAULT_TOOLS = ROOT / ".vault" / "tools"
sys.path.insert(0, str(VAULT_TOOLS))

import yaml  # noqa: E402
from vault_scipy import SCIPY_SPECS, lookup_scipy_spec  # noqa: E402

INCLUDE = ROOT / "include" / "distributions"
BENCH = ROOT / "bench"
CYDIST = ROOT / "cydist"

CATEGORY_MAP = {
    "discrete/finite-support": "discrete/finite",
    "discrete/infinite-support": "discrete/infinite",
    "continuous/bounded-interval": "continuous/bounded",
    "continuous/semi-infinite-interval": "continuous/semi_infinite",
    "continuous/whole-real-line": "continuous/real_line",
    "continuous/variable-support": "continuous/variable",
    "multivariate/continuous": "multivariate/continuous",
    "multivariate/discrete": "multivariate/discrete",
    "multivariate/matrix-valued": "multivariate/matrix",
    "directional/univariate-circular": "directional",
    "directional/multivariate": "directional",
    "directional/bivariate-spherical": "directional",
    "directional/bivariate-toroidal": "directional",
    "degenerate-and-singular/degenerate": "degenerate",
    "degenerate-and-singular/singular": "degenerate",
    "mixed/continuous-discrete": "mixed",
}

# Hand-written headers (moved to category folders); codegen skips these.
MANUAL = {
    "bernoulli": "discrete/finite/bernoulli.hpp",
    "discrete-uniform": "discrete/finite/discrete_uniform.hpp",
    "binomial": "discrete/finite/binomial.hpp",
    "categorical": "discrete/finite/categorical.hpp",
    "beta-binomial": "discrete/finite/beta_binomial.hpp",
    "poisson-binomial": "discrete/finite/poisson_binomial.hpp",
    "zipf": "discrete/finite/zipf.hpp",
    "zipfmandelbrot": "discrete/finite/zipf.hpp",
    "geometric": "discrete/infinite/geometric.hpp",
    "negative-binomial": "discrete/infinite/negative_binomial.hpp",
    "skellam": "discrete/infinite/skellam.hpp",
    "exponential": "continuous/semi_infinite/exponential.hpp",
    "normal-gaussian": "continuous/real_line/normal.hpp",
}

BENCH_ALIAS = {
    "normal-gaussian": "normal",
    "students-t": "student-t",
}

PY_FUNC_ALIASES = {
    "normal-gaussian": "normal_sample_batch",
    "zipfmandelbrot": "zipf_mandelbrot_sample_batch",
}

C_FUNC_ALIASES = {
    "normal-gaussian": "cydist_normal_sample_batch",
    "zipfmandelbrot": "cydist_zipf_mandelbrot_sample_batch",
}

# cydist batch entry points with Cython FusedType (float32 | float64) output.
FUSED_CONTINUOUS_VAULT_IDS = frozenset({"normal-gaussian", "exponential"})

FUSED_FLOAT_CPP_CLASS = {
    "normal-gaussian": "NormalDistribution<float>",
    "exponential": "ExponentialDistribution<float>",
}

MANUAL_CPP_CLASS = {
    "bernoulli": "Bernoulli",
    "discrete-uniform": "DiscreteUniform",
    "binomial": "Binomial",
    "categorical": "Categorical",
    "beta-binomial": "BetaBinomial",
    "poisson-binomial": "PoissonBinomial",
    "zipf": "Zipf",
    "zipfmandelbrot": "ZipfMandelbrot",
    "geometric": "Geometric",
    "negative-binomial": "NegativeBinomial",
    "skellam": "Skellam",
    "exponential": "Exponential",
    "normal-gaussian": "Normal",
}

# C/Cython/Python keywords that cannot be parameter names as-is.
RESERVED_NAMES = frozenset({
    "lambda", "class", "def", "return", "import", "global", "yield", "raise",
    "template", "new", "delete", "operator", "namespace", "struct",
})


