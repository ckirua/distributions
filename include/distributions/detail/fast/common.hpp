#pragma once

#include "distributions/rng.hpp"

#include <cstdint>

namespace distributions::detail {

inline std::uint64_t batch_seed_from(Pcg32& rng) {
    return static_cast<std::uint64_t>(rng.next_u32()) |
           (static_cast<std::uint64_t>(rng.next_u32()) << 32);
}

}  // namespace distributions::detail
