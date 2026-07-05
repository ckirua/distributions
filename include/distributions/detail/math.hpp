#pragma once

#include <cmath>
#include <limits>

namespace distributions::detail {

inline double log1p_neg(double p) {
    return std::log1p(-p);
}

inline bool is_finite(double x) {
    return std::isfinite(x);
}

}  // namespace distributions::detail
