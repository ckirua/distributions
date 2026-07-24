# Future work

## Refactoring direction

The cleanest direction is:

> Turn the repository from “a catalog of fast samplers” into “a capability-aware distribution library with fast samplers.”

The sampling performance work is already valuable. The refactor should preserve that infrastructure while adding mathematical contracts, explicit implementation quality, proper generated metadata, and a higher-level statistical API.

## 1. Define a real distribution contract

The current abstraction is primarily a sampler:

```text
sample(...)
sample_batch(...)
```

Replace the single broad interface with capability-based interfaces:

```cpp
template<class D>
concept Sampler = requires(const D& d, Pcg32& rng) {
    d.sample(rng);
};

template<class D>
concept DiscreteMass = requires(const D& d, int x) {
    d.pmf(x);
    d.log_pmf(x);
    d.cdf(x);
};

template<class D>
concept ContinuousDensity = requires(const D& d, double x) {
    d.pdf(x);
    d.log_pdf(x);
    d.cdf(x);
};

template<class D>
concept Quantile = requires(const D& d, double p) {
    d.quantile(p);
};

template<class D>
concept Moments = requires(const D& d) {
    d.mean();
    d.variance();
};
```

Not every distribution supports every operation. A categorical variable may have a PMF and CDF but no meaningful numeric mean unless labels are numeric.

This avoids pretending that every distribution has valid `mean()` and `variance()`.

## 2. Separate mathematical distributions from algorithms

A distribution should describe its mathematics and parameters. Sampling should be an implementation strategy.

Conceptually:

```cpp
NormalDistribution<double> normal(mu, sigma);
auto x = sample(normal, rng);
```

The distribution object owns:

- Parameters
- Parameter validation
- Support
- `pdf`/`pmf`
- `cdf`
- Quantile
- Moments where mathematically available

The sampling layer owns:

- Inverse transform
- Rejection sampling
- Ziggurat
- Gamma composition
- SIMD implementation
- Batch dispatch

This makes it possible to improve a sampler without rewriting the distribution API.

## 3. Make exactness explicit

The current `family` and `heuristic` tiers are too easy to confuse with production-quality implementations.

Use explicit implementation status:

```cpp
enum class ImplementationStatus {
    exact,
    approximate,
    heuristic,
    experimental
};
```

Expose this in the registry and Python metadata:

```python
cydist.info("generalized_chi_squared")
# {
#   "status": "heuristic",
#   "supports": ["sample"],
#   "validated_against": None
# }
```

The generic normal fallback should not be treated as an implementation of the named distribution. It should either live in an explicitly experimental namespace or be excluded from the public distribution registry until it is mathematically valid.

This is the most important refactor for statistical correctness.

## 4. Replace the current generated/manual split with one source of truth

The project currently has:

- Handwritten distributions
- Generated headers
- Generated Cython code
- Generated C ABI
- Generated tests
- A separate registry
- Recipes with embedded C++ implementation bodies

Make the registry/specification the canonical source:

```yaml
id: normal
kind: continuous
parameters:
  - name: mu
    constraint: finite
  - name: sigma
    constraint: positive
support: real
capabilities:
  - sample
  - pdf
  - log_pdf
  - cdf
  - quantile
  - moments
implementation:
  sampler: ziggurat
  status: exact
validation:
  reference: scipy.stats.norm
```

Generate the following from it:

- C++ declarations and bindings
- Python wrappers
- C ABI
- Capability metadata
- Test registration
- Documentation tables

The recipe should describe algorithms and formulas, not be the only place where semantics are encoded.

## 5. Introduce common parameter validation

Parameter validation is currently distributed among generated Cython checks and C++ constructors.

Standardize it in C++ first:

```cpp
struct Positive {
    static void validate(double x);
};

struct Probability {
    static void validate(double p);
};

struct OrderedInterval {
    static void validate(double lo, double hi);
};
```

Every distribution constructor should validate its own parameters at the C++ boundary. Python validation can remain for fast user feedback, but it must not be the authoritative layer.

This prevents users calling the C ABI or C++ API with invalid parameters and getting inconsistent behavior.

## 6. Refactor Python bindings into distribution objects

Preserve the efficient low-level API:

```python
normal_sample_batch(out, mu, sigma, seed)
```

Add a higher-level API:

```python
dist = cydist.Normal(mu=0.0, sigma=1.0)
samples = dist.rvs(size=1_000_000, seed=42)
values = dist.pdf(x)
log_values = dist.logpdf(x)
```

Internally:

- `rvs()` uses the existing zero-copy batch path
- `pdf`, `cdf`, and `logpdf` use vectorized C++ kernels where worthwhile
- `out=` remains available for allocation-free workflows

There should be two layers:

```text
Low-level: normal_sample_batch(out, ...)
High-level: Normal(...).rvs(...)
```

The low-level API is important for HPC users and should not be removed.

## 7. Treat multivariate distributions separately

Scalar distributions and matrix-valued distributions should not be forced into the same `Sample` model.

Use separate concepts:

```cpp
ScalarDistribution
VectorDistribution
MatrixDistribution
```

A multivariate normal should return a vector or fill a matrix-shaped output buffer. A Wishart distribution should have explicit matrix dimensions and storage semantics.

The scalar-oriented `Sample` abstraction is a poor long-term fit for matrix-valued distributions.

## 8. Build a validation matrix, not one generic sanity test

Organize validation by contract.

### Sampling

- Support bounds
- Empirical moments
- Distribution comparison
- Reproducibility
- Extreme parameter values

### Density/mass

- Non-negativity
- Normalization
- Known special cases
- Log-space consistency

```text
log_pdf(x) ≈ log(pdf(x))
```

### CDF

- Monotonicity
- Range `[0, 1]`
- Boundary values
- Numerical agreement with reference

### Quantile

- Range validity
- Monotonicity
- Round trip:

```text
cdf(quantile(p)) ≈ p
```

### Moments

- Closed-form values
- Empirical agreement
- Explicit “undefined” behavior for infinite moments

Tests should be generated from the registry, but assertions should be capability-specific.

## 9. Separate correctness status from performance tiers

The current Tier A/B/C terminology mixes RNG implementation and distribution quality.

Use two independent dimensions:

```text
Correctness:
  exact / approximate / heuristic

Execution:
  scalar / batch / AVX2 / AVX512
```

For example:

```text
Normal:
  correctness: exact
  execution: scalar, batch, AVX2, AVX512

SomeDistribution:
  correctness: heuristic
  execution: scalar, batch
```

A faster implementation must never implicitly mean a different statistical contract.

## 10. Reorganize the repository around public versus internal code

Make the boundaries clearer:

```text
include/distributions/
  distribution/
    scalar.hpp
    discrete.hpp
    continuous.hpp
    multivariate.hpp
  math/
    normal.hpp
    gamma.hpp
    special_functions.hpp
  algorithms/
    inverse_transform.hpp
    rejection.hpp
    ziggurat.hpp
    simd/
  distributions/
    normal.hpp
    gamma.hpp
    ...
  registry/
    generated.hpp
```

The exact naming is flexible. The key is to distinguish:

- Public distribution types
- Mathematical primitives
- Sampling algorithms
- SIMD internals
- Generated registry code

## Recommended refactor order

### Phase 1: Correctness boundary

1. Audit every distribution into exact, approximate, heuristic, or experimental.
2. Remove fake moment implementations.
3. Separate `Sampler` from `Moments`.
4. Make support and parameter constraints explicit.
5. Ensure C++ constructors validate parameters.

### Phase 2: Public statistical API

1. Add `pmf`/`pdf`.
2. Add `log_pmf`/`log_pdf`.
3. Add `cdf`.
4. Add `quantile` where numerically viable.
5. Add moments only where mathematically defined.

### Phase 3: Registry and generation

1. Make the registry the source of truth.
2. Generate bindings, metadata, and test cases from it.
3. Stop embedding broad generic fallbacks in the public set.
4. Make unsupported capabilities visible instead of silently absent.

### Phase 4: Python usability

1. Keep current low-level batch functions.
2. Add high-level distribution objects.
3. Add `rvs`, `pdf`, `cdf`, `ppf`, and `logpdf`.
4. Support `out=` and configurable dtype.
5. Add vectorized parameter support only after scalar semantics are stable.

### Phase 5: Inference utilities

Only after the distribution API is reliable:

- MLE fitting
- Method-of-moments fitting
- Bootstrap helpers
- Likelihood utilities
- Confidence intervals
- Basic goodness-of-fit tests

Do not add fitting before `pdf`, `logpdf`, `cdf`, and quantiles are reliable.

## What not to do

- Do not rewrite all 189 distributions at once.
- Do not add a large abstraction framework before fixing correctness metadata.
- Do not preserve placeholder samplers under production-looking names.
- Do not make every distribution implement every method.
- Do not sacrifice the low-level zero-copy batch API.
- Do not make Python the source of truth for validation or semantics.
