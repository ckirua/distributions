#!/usr/bin/env python3
"""Build the Cython demo extension (standalone, links cydist_shim from repo root)."""

import os
from pathlib import Path

import numpy as np
from Cython.Build import cythonize
from setuptools import Extension, setup

ROOT = Path(__file__).resolve().parents[2]
CXX = os.environ.get("CXX", "g++-14")
os.environ.setdefault("CC", CXX)
os.environ.setdefault("CXX", CXX)

setup(
    ext_modules=cythonize(
        [
            Extension(
                name="demo",
                sources=[
                    "demo.pyx",
                    str(ROOT / "cydist" / "cydist_shim.cpp"),
                ],
                include_dirs=[
                    str(ROOT / "include"),
                    str(ROOT / "cydist"),
                    np.get_include(),
                ],
                language="c++",
                extra_compile_args=["-std=c++26", "-O3", "-fno-math-errno"],
            )
        ],
        compiler_directives={"language_level": "3"},
    ),
)
