#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>

namespace distributions::detail {

/// Debug-only parameter checks (compiled out when ``NDEBUG`` is set).
inline void assert_probability(double p) {
    assert(p >= 0.0 && p <= 1.0 && std::isfinite(p));
}

inline void assert_open_unit_interval(double p) {
    assert(p > 0.0 && p <= 1.0 && std::isfinite(p));
}

inline void assert_strictly_positive(double x) {
    assert(x > 0.0 && std::isfinite(x));
}

inline void assert_nonnegative(double x) {
    assert(x >= 0.0 && std::isfinite(x));
}

inline void assert_finite(double x) {
    assert(std::isfinite(x));
}

inline void assert_positive_int(int n) {
    assert(n > 0);
}

inline void assert_nonnegative_int(int n) {
    assert(n >= 0);
}

inline void assert_int_interval(int lo, int hi) {
    assert(lo <= hi);
}

/// Generalized hyperbolic: requires ``a > |b|``.
inline void assert_gh_support(double a, double b) {
    assert(std::isfinite(a) && std::isfinite(b) && a > std::abs(b));
}

/// Zipf–Mandelbrot: requires ``q > -1`` so ``k + q > 0`` for ``k >= 1``.
inline void assert_zipf_mandelbrot_q(double q) {
    assert(std::isfinite(q) && q > -1.0);
}

inline void assert_double_interval(double lo, double hi) {
    assert(std::isfinite(lo) && std::isfinite(hi) && lo < hi);
}

}  // namespace distributions::detail
