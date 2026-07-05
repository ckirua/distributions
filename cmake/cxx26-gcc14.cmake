# Prefer GCC 14+ for C++26. Falls back to g++ if g++-14 is absent.
find_program(DISTRIBUTIONS_GXX NAMES g++-14 g++ REQUIRED)
set(CMAKE_CXX_COMPILER "${DISTRIBUTIONS_GXX}" CACHE STRING "C++ compiler" FORCE)
