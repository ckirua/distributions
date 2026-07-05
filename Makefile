# Repo hygiene + common tasks. Requires: g++-14, cmake, .venv with deps installed.

VENV     := $(CURDIR)/.venv/bin
PYTHON   := $(VENV)/python
PIP      := $(VENV)/pip
BUILD    := build
CMAKE    := cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-14
BUILD_SIMD := build-simd
CMAKE_SIMD := cmake -S . -B $(BUILD_SIMD) -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-14 -DDISTRIBUTIONS_ENABLE_SIMD=ON

.PHONY: help clean codegen configure configure-simd build build-simd test test-simd test-sanity test-all bench bench-core bench-core-simd bench-core-baseline bench-core-quick bench-all vault install

help:
	@echo "Targets: clean codegen configure build test bench vault install"
	@echo "  make codegen   — regenerate distribution headers + dispatch + cydist shim"
	@echo "  make build     — configure (if needed) and compile C++ benchmarks"
	@echo "  make build-simd — same with -DDISTRIBUTIONS_ENABLE_SIMD=ON (AVX2 Tier C)"
	@echo "  make test      — ctest + fast pytest smoke (excludes sanity)"
	@echo "  make test-simd — ctest on build-simd (Tier B vs C repro, when AVX2)"
	@echo "  make test-sanity — statistical checks vs scipy (~48 cases, slow)"
	@echo "  make test-all  — smoke + sanity"
	@echo "  make bench     — benchmark 13 hand-written ids (alias for bench-core)"
	@echo "  make bench-core — benchmark 13 hand-written ids (1k/100k/10M)"
	@echo "  make bench-core-simd — bench-core using build-simd/ (Tier C when enabled)"
	@echo "  make bench-core-quick — hand-written ids at 1k/100k only"
	@echo "  make bench-all — benchmark all 189 distributions (C++ timings)"
	@echo "  make vault     — rebuild Obsidian vault notes"
	@echo "  make install   — editable pip install of cydist"

clean:
	rm -rf $(BUILD) cydist.egg-info dist .pytest_cache .mypy_cache .ruff_cache
	rm -rf cydist/__pycache__ cydist/*.cpp cydist/_native*.so
	rm -rf tools/__pycache__ .vault/tools/__pycache__
	find . -type d -name __pycache__ -not -path './.venv/*' -exec rm -rf {} + 2>/dev/null || true

codegen:
	$(PYTHON) tools/codegen_distributions.py

configure:
	$(CMAKE)

configure-simd:
	$(CMAKE_SIMD)

build: configure
	cmake --build $(BUILD) -j$$(nproc)

build-simd: configure-simd
	cmake --build $(BUILD_SIMD) -j$$(nproc)

test: build install
	ctest --test-dir $(BUILD) --output-on-failure
	$(PYTHON) -m pytest tests/ -q --tb=line

test-simd: build-simd install
	ctest --test-dir $(BUILD_SIMD) --output-on-failure
	$(PYTHON) -m pytest tests/test_reproducibility_simd.py -q --tb=line

test-sanity: install
	$(PYTHON) -m pytest tests/ -m sanity -q --tb=line -o addopts=

test-all: build install
	ctest --test-dir $(BUILD) --output-on-failure
	$(PYTHON) -m pytest tests/ -q --tb=line -o addopts=

bench: build
	$(PYTHON) bench/bench_core.py

bench-core: build
	$(PYTHON) bench/bench_core.py

bench-core-baseline: build
	$(PYTHON) bench/bench_core.py --out $(CURDIR)/results/baseline-v0.3.0

bench-core-simd: build-simd
	$(PYTHON) bench/bench_core.py --run-bench $(BUILD_SIMD)/run_bench --out $(CURDIR)/results/current

bench-core-quick: build
	$(PYTHON) bench/bench_core.py --quick

bench-all: build
	$(PYTHON) bench/sweep.py --all --skip-verify --quick
	$(PYTHON) bench/aggregate_summary.py

vault:
	$(PYTHON) .vault/tools/build_vault.py

install:
	$(PIP) install -e .

examples-cpp: build
	./$(BUILD)/example_cpp_basic
	./$(BUILD)/example_c_api

examples-python: install
	$(PYTHON) examples/python/basic.py
	$(PYTHON) examples/python/pandas_demo.py
	$(PYTHON) examples/python/polars_demo.py
	$(PYTHON) examples/python/matplotlib_demo.py

examples-cython: install
	cd examples/cython && $(PYTHON) setup_demo.py build_ext --inplace
	cd examples/cython && $(PYTHON) run_demo.py

examples: examples-cpp examples-python examples-cython
