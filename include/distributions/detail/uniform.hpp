#pragma once

#include "distributions/rng.hpp"

namespace distributions::detail {

inline double sample_uniform(Pcg32& rng, double lo, double hi) {
    return lo + (hi - lo) * rng.next_double();
}

inline int sample_uniform_int(Pcg32& rng, int lo, int hi) {
    const int span = hi - lo + 1;
    return lo + static_cast<int>(rng.next_u32() % static_cast<std::uint32_t>(span));
}

}  // namespace distributions::detail
