#pragma once

#include "distributions/detail/chi_squared.hpp"
#include "distributions/rng.hpp"

namespace distributions::detail {

inline double sample_f(Pcg32& rng, double dfn, double dfd) {
    const double x = sample_chi_squared(rng, dfn);
    const double y = sample_chi_squared(rng, dfd);
    return (x / dfn) / (y / dfd);
}

}  // namespace distributions::detail
