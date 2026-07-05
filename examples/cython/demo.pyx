# Example: Cython — cimport NumPy, call C shim with nogil.
# Build: see examples/cython/setup_demo.py

cimport numpy as cnp
from libc.stddef cimport size_t
from libc.stdint cimport uint64_t

cnp.import_array()

cdef extern from "cydist_shim.h":
    void cydist_poisson_sample_batch(
        double mu, uint64_t seed, int* out, size_t n_samples) nogil
    void cydist_normal_sample_batch(
        double mu, double sigma, uint64_t seed, double* out, size_t n_samples) nogil


def poisson_demo(cnp.int32_t[:] out, double mu, uint64_t seed=42):
    """Fill `out` with Poisson(mu) samples (GIL released during sampling)."""
    cdef int* ptr = <int*>&out[0]
    cdef size_t n = <size_t>out.shape[0]
    with nogil:
        cydist_poisson_sample_batch(mu, seed, ptr, n)


def normal_demo(cnp.float64_t[:] out, double mu, double sigma, uint64_t seed=42):
    """Fill `out` with Normal(mu, sigma) samples."""
    cdef double* ptr = <double*>&out[0]
    cdef size_t n = <size_t>out.shape[0]
    with nogil:
        cydist_normal_sample_batch(mu, sigma, seed, ptr, n)
