import os
from pathlib import Path

import numpy as np
from picobuild import cythonize, Extension, setup

ROOT = Path(__file__).parent
CXX = os.environ.get("CXX", "g++-14")
os.environ.setdefault("CC", CXX)
os.environ.setdefault("CXX", CXX)

setup(
    ext_modules=cythonize(
        [
            Extension(
                name="cydist._native",
                sources=["cydist/cydist.pyx", "cydist/cydist_shim.cpp"],
                include_dirs=[str(ROOT / "include"), str(ROOT / "cydist")],
                language="c++",
                extra_compile_args=["-std=c++26", "-O3", "-fno-math-errno"],
            )
        ],
        compiler_directives={"language_level": "3"},
    ),
    packages=["cydist"],
    package_data={"cydist": ["__init__.pyi"]},
    include_dirs=[np.get_include()],
)
