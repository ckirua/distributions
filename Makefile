# Repo hygiene + common tasks. Requires: g++-14, cmake, .venv with deps installed.

VENV     := $(CURDIR)/.venv/bin
PYTHON   := $(VENV)/python
PIP      := $(VENV)/pip
BUILD    := build
CMAKE    := cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-14

.PHONY: help clean codegen configure build test bench bench-all vault install

help:
	@echo "Targets: clean codegen configure build test bench vault install"
	@echo "  make codegen   — regenerate distribution headers + dispatch + cydist shim"
	@echo "  make build     — configure (if needed) and compile C++ benchmarks"
	@echo "  make test      — run ctest"
	@echo "  make bench     — sweep Phase-1 ISPC candidates"
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

build: configure
	cmake --build $(BUILD) -j$$(nproc)

test: build install
	ctest --test-dir $(BUILD) --output-on-failure
	$(PYTHON) -m pytest tests/ -q --tb=line

bench: build
	$(PYTHON) bench/sweep.py

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

examples-cython: install
	cd examples/cython && $(PYTHON) setup_demo.py build_ext --inplace
	cd examples/cython && $(PYTHON) run_demo.py

examples: examples-cpp examples-python examples-cython
