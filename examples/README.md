# Examples

Minimal usage of the sampler library from C++, C, Python, and Cython.

## Prerequisites

From repo root:

```bash
.venv/bin/pip install -r .vault/requirements.txt
make install          # editable cydist
make build            # C++ examples (CMake)
```

## C++ (headers)

Direct use of `distributions::*` structs — lowest latency, no Python involved.

```bash
./build/example_cpp_basic
```

Source: [`cpp/basic.cpp`](cpp/basic.cpp)

## C (stable C ABI)

Call `cydist_*_sample_batch` from `cydist/cydist_shim.h` — useful for FFI.

```bash
./build/example_c_api
```

Source: [`cpp/c_api.c`](cpp/c_api.c)

## Python

NumPy buffers filled in-place via `cydist` (C++ backend, releases the GIL inside each call).

```bash
.venv/bin/pip install -e ".[examples]"   # matplotlib, pandas, polars
make install
.venv/bin/python examples/python/basic.py
.venv/bin/python examples/python/matplotlib_demo.py   # → poisson_normal_hist.png
.venv/bin/python examples/python/pandas_demo.py
.venv/bin/python examples/python/polars_demo.py
# or: make examples-python
```

| Script | Shows |
|--------|--------|
| [`python/basic.py`](python/basic.py) | Minimal NumPy in-place sampling |
| [`python/matplotlib_demo.py`](python/matplotlib_demo.py) | Histograms of Poisson + Normal samples |
| [`python/pandas_demo.py`](python/pandas_demo.py) | `pandas.DataFrame` from multiple samplers |
| [`python/polars_demo.py`](python/polars_demo.py) | `polars.DataFrame` from discrete samplers |

## Cython

`cimport numpy`, declare `extern from "cydist_shim.h"`, call samplers with `nogil`.

```bash
cd examples/cython
../../.venv/bin/python setup_demo.py build_ext --inplace
../../.venv/bin/python run_demo.py
```

Source: [`cython/demo.pyx`](cython/demo.pyx)

## Pattern summary

| Layer | Include / import | Fill buffer |
|-------|------------------|-------------|
| C++ | `#include "distributions/.../poisson.hpp"` | `dist.sample_batch(out, n, rng)` |
| C | `#include "cydist_shim.h"` | `cydist_poisson_sample_batch(...)` |
| Python | `from cydist import poisson_sample_batch` | `poisson_sample_batch(out, mu, seed=42)` |
| Cython | `cdef extern from "cydist_shim.h"` + `cimport numpy` | same C calls, `with nogil:` |

All batch functions write into a **pre-allocated** output array; they do not allocate per call.
